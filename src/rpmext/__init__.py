
import rpm
import _rpmext
from rpm._rpm import ts as _rpmts

#
# This will be used to override rpm.transaction.TransactionSet.addInstall()
# Depending on the the 'prefix' argument it will either call the original
# version, or the new version from _rpmext.
#
def addInstall(self, item, key, how='u', prefix = None):

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

    if prefix and prefix != '':
        prefixes = header['prefixes']
        if len(prefixes) > 0:
            oldpath = prefixes[0]
            if not _rpmext.addInstall(self, header, key, upgrade, oldpath, prefix):
                raise rpm.error("adding package to transaction failed")
            return

    if not _rpmts.addInstall(self, header, key, upgrade):
        raise rpm.error("adding package to transaction failed")
   
