
import rpm
import _rpmext
from rpm._rpm import ts as _rpmts

#
# This will be used to override rpm.transaction.TransactionSet.addInstall()
# Depending on the the 'relocs' argument it will either call the original
# version, or the new version from _rpmext.
#
# 'relocs' can be single string, in which case it is assumed to be a 
# new prefix and the oldpath is taken from the package header prefix,
# or 'relocs' can be a list, in which case it must contain 2-tuples of strings
# representing (oldpath, newpath) pairs.
#
def addInstall(self, item, key, how='u', relocs = None):

    if isinstance(item, str):
        f = file(item)
        header = self.hdrFromFdno(f)
        f.close()
    elif isinstance(item, file):
        header = self.hdrFromFdno(item)
    else:
        header = item

    if not how in ['u', 'i']:
        raise ValueError('how argument must be "u" or "i"')

    upgrade = (how == "u")

    if relocs:
        if type(relocs) == str:
            prefixes = header['prefixes']
            if relocs == '':
                relocs = []
            elif len(prefixes) > 0:
                relocs = [(prefixes[0], relocs)]
            else:
                raise ValueError('prefix given, but package is not relocatable')

        elif type(relocs) != list:
            raise ValueError("relocs argument must be either a string or a list");

        print "relocations:",relocs

        if not _rpmext.addInstall(self, header, key, upgrade, relocs):
            raise rpm.error("adding package to transaction failed")
        return

    if not _rpmts.addInstall(self, header, key, upgrade):
        raise rpm.error("adding package to transaction failed")
   
