#------------------------------------------------------------------------------

                             OPENAIR-CN
    An implementation of the Evolved Packet Core network.

#------------------------------------------------------------------------------

Openair5g-cn is an implementation of the 3GPP specifications concerning the
Evolved Packet Core Networks for 5G Systems.

# Licence info

    Work in progress ...

# Linux

The Openair5g-cn code actually is written, executed, and tested on UBUNTU desktop xenial,*

# Collaborative work

This source code is managed through a GITHUB, a collaborative development
platform, URL: https://github.com/OPENAIRINTERFACE/openair5g-cn

# Contributions request

In a general way, anybody who is willing can contribute on any part of the
code in any network component. Contributions can be simple bugfixes, advices
and remarks on the design, architecture, coding/implementation, are can can
size up to a project collaboration.

# How to contribute

Please visit https://www.openairinterface.org/?page_id=112

# Build instructions

## Prerequisites

The build system needs to set up the OPENAIR5GCN_DIR environment variable

You can set up this variable as is:

    . oaienv

## Dependencies for amf

The build needs some library dependancies.

For your convenience, a build script is provided to install these dependencies.
You can launch it with the following command:

    ./scripts/build_amf -f -i

## Compile the amf

    ./scripts/build_amf -c
## Compile Issue
    struct redifination in json.hpp (/usr/local/**/json.hpp && /src/amf/AmfGern/**/json.hpp) 
                ======> mv /usr/local/**/json.hpp to json_bak.hpp ; recompile
## Run amf
   ./scripts/run_amf  ===> ./build/***/amf directly
