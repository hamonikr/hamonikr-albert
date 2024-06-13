// Copyright (c) 2022-2024 Manuel Schneider

#pragma once
#include "albert/export.h"
#include <QString>

namespace albert
{
class PluginInstance;
class PluginMetaData;

///
/// Abstract plugin loader class.
///
/// @since 0.23
///
class ALBERT_EXPORT PluginLoader
{
public:
    virtual QString path() const = 0;

    /// The plugin metadata.
    /// @returns The PluginMetaData of the plugin.
    virtual const PluginMetaData &metaData() const = 0;

    /// Load the plugin.
    /// @note Will be executed in a background thread.
    virtual void load() = 0;

    /// Unload the plugin.
    /// @note Will be executed in a background thread.
    virtual void unload() = 0;

    /// Create the plugin instance.
    /// @returns The PluginInstance or nullptr if not loaded.
    virtual PluginInstance *createInstance() = 0;

protected:

    virtual ~PluginLoader() = default;

};

}
