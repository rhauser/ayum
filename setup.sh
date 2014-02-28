export AYUM_PATH=$(readlink -f $(dirname $BASH_SOURCE))
export PATH=${AYUM_PATH}/bin:$PATH
