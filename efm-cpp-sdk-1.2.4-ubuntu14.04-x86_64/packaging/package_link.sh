#!/bin/bash

set -e

argc=$#

if [ "${argc}" -lt 4 ]; then
    echo 'package_link.sh <name> <executable> <version> <description> [libs]'
    exit 1
fi

command -v zip || { echo "could not find command 'zip'"; exit 1; }

cwd=$(dirname "$(readlink -f "$0")")

name=$1
executable=$2
version=$3
description=$4
arch=x86_64

if [ -z "$name" ]; then
    echo "name parameter is empty"
    exit 1
fi
if [ -z "$executable" ]; then
    echo "executable parameter is empty"
    exit 1
fi
if [ ! -f "$executable" ]; then
    echo "executable '$executable' does not exist"
    exit 1
fi
if [ -z "$version" ]; then
    echo "version parameter is empty"
    exit 1
fi

link_name=cpp-${name}
link_pkg_name=dslink-${link_name}
package_name=${link_pkg_name}_${version}_${arch}

echo "using package name '${link_pkg_name}'"
echo "using link name '${link_name}'"

tmpdir=$(mktemp -d -t package_link_XXXXXXXX)

# delete the temporary folder on exit
trap "rm -rf ${tmpdir}" EXIT

pkgdir=${tmpdir}/${package_name}
mkdir -p ${pkgdir}/bin ${pkgdir}/lib

cp ${executable} ${pkgdir}/bin
for (( i=5; i < argc+1; i++ )); do
    lib=${!i}
    cp ${lib} ${pkgdir}/lib/$(basename ${lib})
done

sed s/%%EXECUTABLE%%/$(basename ${executable})/g ${cwd}/start-link.sh.template > ${pkgdir}/bin/start-link.sh
sed "s/%%PKGNAME%%/$link_pkg_name/g; s/%%NAME%%/$link_name/g; s/%%VERSION%%/$version/g; s/%%DESCRIPTION%%/$description/g" ${cwd}/dslink.json.template > ${pkgdir}/dslink.json

(cd ${pkgdir} && zip -r - *) > ${package_name}.zip