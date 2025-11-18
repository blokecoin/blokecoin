# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libblokecoin_cli*         | RPC client functionality used by *blokecoin-cli* executable |
| *libblokecoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libblokecoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libblokecoin_consensus*   | Consensus functionality used by *libblokecoin_node* and *libblokecoin_wallet*. |
| *libblokecoin_crypto*      | Hardware-optimized functions for data encryption, hashing, message authentication, and key derivation. |
| *libblokecoin_kernel*      | Consensus engine and support library used for validation by *libblokecoin_node*. |
| *libblokecoinqt*           | GUI functionality used by *blokecoin-qt* and *blokecoin-gui* executables. |
| *libblokecoin_ipc*         | IPC functionality used by *blokecoin-node* and *blokecoin-gui* executables to communicate when [`-DENABLE_IPC=ON`](multiprocess.md) is used. |
| *libblokecoin_node*        | P2P and RPC server functionality used by *blokecoind* and *blokecoin-qt* executables. |
| *libblokecoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libblokecoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libblokecoin_wallet*      | Wallet functionality used by *blokecoind* and *blokecoin-wallet* executables. |
| *libblokecoin_wallet_tool* | Lower-level wallet functionality used by *blokecoin-wallet* executable. |
| *libblokecoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *blokecoind* and *blokecoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. An exception is *libblokecoin_kernel*, which, at some future point, will have a documented external interface.

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`add_library(blokecoin_* ...)`](../../src/CMakeLists.txt) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libblokecoin_node* code lives in `src/node/` in the `node::` namespace
  - *libblokecoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libblokecoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libblokecoin_util* code lives in `src/util/` in the `util::` namespace
  - *libblokecoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

blokecoin-cli[blokecoin-cli]-->libblokecoin_cli;

blokecoind[blokecoind]-->libblokecoin_node;
blokecoind[blokecoind]-->libblokecoin_wallet;

blokecoin-qt[blokecoin-qt]-->libblokecoin_node;
blokecoin-qt[blokecoin-qt]-->libblokecoinqt;
blokecoin-qt[blokecoin-qt]-->libblokecoin_wallet;

blokecoin-wallet[blokecoin-wallet]-->libblokecoin_wallet;
blokecoin-wallet[blokecoin-wallet]-->libblokecoin_wallet_tool;

libblokecoin_cli-->libblokecoin_util;
libblokecoin_cli-->libblokecoin_common;

libblokecoin_consensus-->libblokecoin_crypto;

libblokecoin_common-->libblokecoin_consensus;
libblokecoin_common-->libblokecoin_crypto;
libblokecoin_common-->libblokecoin_util;

libblokecoin_kernel-->libblokecoin_consensus;
libblokecoin_kernel-->libblokecoin_crypto;
libblokecoin_kernel-->libblokecoin_util;

libblokecoin_node-->libblokecoin_consensus;
libblokecoin_node-->libblokecoin_crypto;
libblokecoin_node-->libblokecoin_kernel;
libblokecoin_node-->libblokecoin_common;
libblokecoin_node-->libblokecoin_util;

libblokecoinqt-->libblokecoin_common;
libblokecoinqt-->libblokecoin_util;

libblokecoin_util-->libblokecoin_crypto;

libblokecoin_wallet-->libblokecoin_common;
libblokecoin_wallet-->libblokecoin_crypto;
libblokecoin_wallet-->libblokecoin_util;

libblokecoin_wallet_tool-->libblokecoin_wallet;
libblokecoin_wallet_tool-->libblokecoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class blokecoin-qt,blokecoind,blokecoin-cli,blokecoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Crypto* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus, crypto, and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libblokecoin_wallet* and *libblokecoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libblokecoin_crypto* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libblokecoin_consensus* should only depend on *libblokecoin_crypto*, and all other libraries besides *libblokecoin_crypto* should be allowed to depend on it.

- *libblokecoin_util* should be a standalone dependency that any library can depend on, and it should not depend on other libraries except *libblokecoin_crypto*. It provides basic utilities that fill in gaps in the C++ standard library and provide lightweight abstractions over platform-specific features. Since the util library is distributed with the kernel and is usable by kernel applications, it shouldn't contain functions that external code shouldn't call, like higher level code targeted at the node or wallet. (*libblokecoin_common* is a better place for higher level code, or code that is meant to be used by internal applications only.)

- *libblokecoin_common* is a home for miscellaneous shared code used by different Blokecoin Core applications. It should not depend on anything other than *libblokecoin_util*, *libblokecoin_consensus*, and *libblokecoin_crypto*.

- *libblokecoin_kernel* should only depend on *libblokecoin_util*, *libblokecoin_consensus*, and *libblokecoin_crypto*.

- The only thing that should depend on *libblokecoin_kernel* internally should be *libblokecoin_node*. GUI and wallet libraries *libblokecoinqt* and *libblokecoin_wallet* in particular should not depend on *libblokecoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be able to get it from *libblokecoin_consensus*, *libblokecoin_common*, *libblokecoin_crypto*, and *libblokecoin_util*, instead of *libblokecoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libblokecoinqt*, *libblokecoin_node*, *libblokecoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](../../src/interfaces/) abstract interfaces.

## Work in progress

- Validation code is moving from *libblokecoin_node* to *libblokecoin_kernel* as part of [The libblokecoinkernel Project #27587](https://github.com/blokecoin/blokecoin/issues/27587)
