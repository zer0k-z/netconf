# Source 2 Network Configurer

Metamod plugin that provides an unlocked alternative to net_option, useful for listen servers.

### Prerequisites
 * This repository is cloned recursively (ie. has submodules)
 * [python3](https://www.python.org/)
 * [ambuild](https://github.com/alliedmodders/ambuild), make sure ``ambuild`` command is available via the ``PATH`` environment variable;
 * MSVC (VS build tools)/Clang installed for Windows/Linux.

### Setting up
 * ``mkdir build`` & ``cd build`` in the root of the plugin folder.
 * Run ``python3 ../configure.py``.
 * If the process of configuring was successful, you should be able to run ``ambuild`` in the ``\build`` folder to compile the plugin.
 * Once the plugin is compiled the files would be packaged and placed in ``\build\package`` folder.
 * To run the plugin on the server, place the files preserving the layout provided in ``\package``. Be aware that plugins get loaded either by corresponding ``.vdf`` files (automatic step) in the metamod folder, or by listing them in ``addons/metamod/metaplugins.ini`` file (manual step).
