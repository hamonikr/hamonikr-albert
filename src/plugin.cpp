// Copyright (c) 2024 Manuel Schneider

#include "albert/albert.h"
#include "albert/extension/pluginprovider/plugininstance.h"
#include "albert/extension/pluginprovider/pluginloader.h"
#include "albert/extension/pluginprovider/pluginmetadata.h"
#include "albert/logging.h"
#include "plugin.h"
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>
#include <chrono>
using namespace albert;
using namespace std::chrono;
using namespace std;


// referenced in plugininstance.cpp
PluginLoader *instanciated_loader;


Plugin::Plugin(PluginProvider *provider_, PluginLoader *loader_)
    : provider(provider_), loader(loader_), state_(State::Unloaded)
{
    enabled_ = settings()->value(QString("%1/enabled").arg(id()), false).toBool();

    const auto &md = loader->metaData();

    static const auto regex_id = QRegularExpression("[a-z0-9_]");
    if (!regex_id.match(md.id).hasMatch())
        throw std::runtime_error(tr("Invalid plugin id. Use [a-z0-9_].").toStdString());

    static const QRegularExpression regex_version(R"R(^(\d+)(?>\.(\d+))?\.(\d+)$)R");
    if (!regex_version.match(md.version).hasMatch())
        WARN << id() << "metadata: Invalid version scheme. Use '<major>.[<minor>.]<patch>'.";

    if (md.name.isEmpty())
        WARN << id() << "metadata: Name should not be empty.";

    if (md.description.isEmpty())
        WARN << id() << "metadata: Description should not be empty.";

    if (md.license.isEmpty())
        WARN << id() << "metadata: License should not be empty.";

    if (md.url.isEmpty())
        WARN << id() << "metadata: URL should not be empty.";

    if (md.authors.isEmpty())
        WARN << id() << "metadata: Authors should not be empty.";
}

QString Plugin::path() const { return loader->path(); }

bool Plugin::isUser() const
{ return loader->metaData().load_type == PluginMetaData::LoadType::User; }

bool Plugin::isEnabled() const { return enabled_; }

void Plugin::setEnabled(bool enable)
{
    if (isUser())
    {
        settings()->setValue(QString("%1/enabled").arg(id()), enabled_ = enable);
        emit enabledChanged();
    }
}

Plugin::State Plugin::state() const { return state_; }

const QString &Plugin::stateInfo() const { return state_info_; }

void Plugin::setState(State state, QString info)
{
    state_ = state;
    state_info_ = info;
    emit stateChanged();
}

QString Plugin::localStateString() const
{
    switch (state_) {
    case Plugin::State::Invalid:
        return Plugin::tr("Plugin is invalid.");
    case Plugin::State::Unloaded:
        return Plugin::tr("Plugin is unloaded.");
    case Plugin::State::Loaded:
        return Plugin::tr("Plugin is loaded.");
    case Plugin::State::Busy:
        return Plugin::tr("Plugin is busy: %1").arg(state_info_);
    }
    return {};
}

PluginInstance *Plugin::instance() const { return instance_; }

QString Plugin::load(albert::ExtensionRegistry &extension_registry) noexcept
{
    if (state_ != State::Unloaded)
        return localStateString();

    setState(State::Busy, tr("Loading…"));

    QStringList errors;

    try
    {
        auto tp = system_clock::now();
        loader->load();
        auto dur_l = duration_cast<milliseconds>(system_clock::now() - tp).count();
        DEBG << QStringLiteral("%1 ms spent loading plugin '%2'").arg(dur_l).arg(id());

        try
        {
            tp = system_clock::now();
            instanciated_loader = loader;
            instance_ = loader->createInstance();
            auto dur_c = duration_cast<milliseconds>(system_clock::now() - tp).count();
            DEBG << QStringLiteral("%1 ms spent instanciating plugin '%2'").arg(dur_c).arg(id());

            if (!instance_)
                throw runtime_error("createInstance() returned nullptr");

            // Get dependencies
            map<QString, PluginInstance*> dependencies;
            for (const auto &d : dependencies_)
            {
                if (d->state() != State::Loaded)
                {
                    auto msg = tr("Dependency '%1' is not initialized.").arg(d->id());
                    throw runtime_error(msg.toStdString());
                }
                dependencies[d->id()] = d->instance_;
            }

            tp = system_clock::now();
            instance_->initialize(extension_registry, dependencies);
            auto dur_i = duration_cast<milliseconds>(system_clock::now() - tp).count();
            DEBG << QStringLiteral("%1 ms spent initializing plugin '%2'").arg(dur_i).arg(id());

            setState(State::Loaded, tr("Load: %1 ms, Instanciate: %2 ms, Initialize: %3 ms.")
                                        .arg(dur_l).arg(dur_c).arg(dur_i));
            return {};

        }
        catch (const exception& e)
        {
            errors << e.what();
            try {
                loader->unload();
            }
            catch (const exception& ue) {
                errors << ue.what();
            }
        }
    }
    catch (const exception& e) { errors << e.what(); }
    catch (...){ errors << tr("Unknown exception occurred."); }

    auto err = errors.join("\n");
    setState(State::Unloaded, err);
    return err;
}

QString Plugin::unload(albert::ExtensionRegistry &extension_registry) noexcept
{
    if (state_ == State::Unloaded){
        setState(State::Unloaded);  // reset state_info
        return {};
    }

    if (state_ != State::Loaded)
        return localStateString();

    setState(State::Busy, tr("Unloading…"));

    QStringList errors;
    try {
        auto tp = system_clock::now();
        instance_->finalize(extension_registry);
        auto dur = duration_cast<milliseconds>(system_clock::now() - tp).count();
        DEBG << QStringLiteral("%1 ms spent finalizing plugin '%2'").arg(dur).arg(id());
    }
    catch (const exception& e) { errors << e.what(); }
    catch (...){ errors << tr("Unknown exception occurred."); }

    instance_ = nullptr;

    try {
        auto tp = system_clock::now();
        loader->unload();
        auto dur = duration_cast<milliseconds>(system_clock::now() - tp).count();
        DEBG << QStringLiteral("%1 ms spent unloading plugin '%2'").arg(dur).arg(id());
    }
    catch (const exception& e) { errors << e.what(); }
    catch (...){ errors << tr("Unknown exception occurred."); }

    setState(State::Unloaded);

    return errors.join("\n");
}

const QString &Plugin::id() const { return loader->metaData().id; }

const PluginMetaData &Plugin::metaData() const { return loader->metaData(); }

set<Plugin*> Plugin::transitiveDependencies() const
{
    set<Plugin*> dependencies = dependencies_;
    for (const auto &dependency : dependencies_)
        dependencies.merge(dependency->transitiveDependencies());
    return dependencies;
}

set<Plugin*> Plugin::transitiveDependees() const
{
    set<Plugin*> dependees = dependees_;
    for (const auto &dependee : dependees_)
        dependees.merge(dependee->transitiveDependees());
    return dependees;
}
