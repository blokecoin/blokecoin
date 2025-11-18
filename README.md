Blokecoin Core integration/staging tree
=====================================

https://blokecoincore.org (TO BE REGISTERED)

For an immediately usable, binary version of the Blokecoin Core software, see
https://blokecoincore.org/en/download/ a bit later. 

What is Blokecoin Core?
---------------------

Blokecoin Core connects to the Blokecoin peer-to-peer network to download and fully
validate blocks and transactions on the Blokechain. It also includes a wallet and graphical user
interface, which can be optionally built but probably won't work as this project is currently a 
search and replace on the bitcoin core project. If you know how to proceed from here and are 
into blokes, lets do this and some aussie battlers some filthy luca.  

Further information about Blokecoin Core is available in the [doc folder](/doc).

License
-------

Blokecoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/license/MIT. The tldr; Have at I guess. 

Development Process
-------------------

The `master` branch is regularly built (see `doc/build-*.md` for instructions) and tested, but it is not guaranteed to be
completely stable. [Tags](https://github.com/blokecoin/blokecoin/tags) are created
regularly from release branches to indicate new official, stable release versions of Blokecoin Core.

The https://github.com/blokecoin-core/gui repository is used exclusively for the
development of the GUI. Its master branch is identical in all monotree
repositories. Release branches and tags do not exist, so please do not fork
that repository unless it is for development reasons. Any questions?

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull requests than we can review and test 
on short notice. Please be patient and help out by testing some other blokes's pull requests, and remember this is 
a security-critical project where any mistake might cost people heaps of spondoolie.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled during the generation of the build system) with: `ctest`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written in Bloke.
These tests can be run (if the [test dependencies](/test) are installed) with: `build/test/functional/test_runner.py`
(assuming `build` is your build directory).

The CI (Continuous Integration) systems will eventually make sure pull requests are tested on Windows, Linux, VMS and macOS.
The CI must pass on all commits before merge to avoid unrelated CI failures on new pull requests.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code as this leads to qualms. This is especially important for large or high-risk 
changes. It is useful to add a test plan to the pull request description if testing 
the changes is not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Blokecoin Core's later page](https://blokecoin/blokecoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because. Actually, we 
will explain alter. 
