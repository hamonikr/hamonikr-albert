// Copyright (c) 2023-2024 Manuel Schneider

#include "albert/albert.h"
#include "albert/extension/pluginprovider/plugininstance.h"
#include "albert/extension/pluginprovider/pluginloader.h"
#include "albert/extension/pluginprovider/pluginmetadata.h"
#include "albert/extensionregistry.h"
#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QSettings>
using namespace albert;
using namespace std;

extern PluginLoader *instanciated_loader;


class PluginInstance::Private
{
public:
    PluginLoader const * const loader{instanciated_loader};
};


PluginInstance::PluginInstance() : d(make_unique<Private>()) {}

PluginInstance::~PluginInstance() {}

QString PluginInstance::id() const
{ return d->loader->metaData().id; }

QString PluginInstance::name() const
{ return d->loader->metaData().name; }

QString PluginInstance::description() const
{ return d->loader->metaData().description; }

static QDir make_dir(const QString &location, const QString &id)
{
    auto dir = QDir(location);
    if (!dir.cd(id)){
        if (!dir.mkpath(id))
            qFatal("Failed to create writable dir at: %s", qPrintable(dir.filePath(id)));
        if (!dir.cd(id))
            qFatal("Failed to cd to just created dir at: %s", qPrintable(dir.filePath(id)));
    }
    return dir;
}

QDir albert::PluginInstance::cacheDir() const
{ return make_dir(albert::cacheLocation(), id()); }

QDir albert::PluginInstance::configDir() const
{ return make_dir(albert::configLocation(), id()); }

QDir albert::PluginInstance::dataDir() const
{ return make_dir(albert::dataLocation(), id()); }

unique_ptr<QSettings> albert::PluginInstance::settings() const
{
    auto s = albert::settings();
    s->beginGroup(id());
    return s;
}

unique_ptr<QSettings> albert::PluginInstance::state() const
{
    auto s = albert::state();
    s->beginGroup(id());
    return s;
}

void PluginInstance::initialize(ExtensionRegistry&, map<QString,PluginInstance*>) {}

void PluginInstance::finalize(ExtensionRegistry&) {}

QWidget *PluginInstance::buildConfigWidget() { return nullptr; }

