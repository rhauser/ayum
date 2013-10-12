from yum.plugins import PluginYumExit, TYPE_CORE, TYPE_INTERACTIVE

requires_api_version = '2.3'
plugin_type = (TYPE_CORE, TYPE_INTERACTIVE)

def init_hook(conduit):
    print "Init"

def config_hook(conduit):
    #help(conduit)
    dbpath = conduit.confString('main','dbpath','')
    if dbpath != '':
      from rpm import addMacro
      addMacro('_dbpath',dbpath)
