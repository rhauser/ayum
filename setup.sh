export AYUM_PATH=$(readlink -f $(dirname $BASH_SOURCE))
alias ayum=${AYUM_PATH}/ayum
export PATH=${AYUM_PATH}/bin:$PATH
