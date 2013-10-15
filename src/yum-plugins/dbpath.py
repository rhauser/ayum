"""
YUM plugin to configure RPM dbpath and prefix value.
The dbpath option should be in the [main] section, while
the prefix option can be used both in the [main] and the [repo]
sections.
"""

from yum import config
from yum.plugins import PluginYumExit, TYPE_CORE, TYPE_INTERACTIVE

requires_api_version = '2.3'
plugin_type = (TYPE_CORE, TYPE_INTERACTIVE)

def config_hook(conduit):
    config.YumConf.dbpath = config.Option('')
    config.YumConf.prefix = config.Option('')
    config.RepoConf.prefix = config.Inherit(config.YumConf.prefix)

def init_hook(conduit):
    conf = conduit.getConf();
    dbpath = conf.dbpath
    if dbpath != '':
      from rpm import addMacro
      addMacro('_dbpath',dbpath)
