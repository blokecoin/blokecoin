# Copyright (c) 2023-present The Blokecoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/license/mit/.

include(AddWindowsResources)

configure_file(${PROJECT_SOURCE_DIR}/cmake/blokecoin-build-config.h.in blokecoin-build-config.h USE_SOURCE_PERMISSIONS @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#=============================
# Subprojects
#=============================
# Subprojects include subdirectories that do or could have tests
# and/or benchmark binaries, such as all subtrees and univalue.
# These need to be included before CMAKE_*_OUTPUT_DIRECTORY variables
# are set, so output locations of subproject tests and libraries are
# not overridden.
include(../cmake/crc32c.cmake)
include(../cmake/leveldb.cmake)
include(../cmake/minisketch.cmake)
add_subdirectory(univalue)
if (ENABLE_IPC AND NOT WITH_EXTERNAL_LIBMULTIPROCESS)
  include(../cmake/libmultiprocess.cmake)
  add_libmultiprocess(ipc/libmultiprocess)
endif()
include(../cmake/secp256k1.cmake)
add_secp256k1(secp256k1)

# Set top-level target output locations.
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

add_custom_target(generate_build_info
  BYPRODUCTS ${PROJECT_BINARY_DIR}/src/blokecoin-build-info.h
  COMMAND ${CMAKE_COMMAND} -DBUILD_INFO_HEADER_PATH=${PROJECT_BINARY_DIR}/src/blokecoin-build-info.h -DSOURCE_DIR=${PROJECT_SOURCE_DIR} -P ${PROJECT_SOURCE_DIR}/cmake/script/GenerateBuildInfo.cmake
  COMMENT "Generating blokecoin-build-info.h"
  VERBATIM
)
add_library(blokecoin_clientversion STATIC EXCLUDE_FROM_ALL
  clientversion.cpp
)
target_link_libraries(blokecoin_clientversion
  PRIVATE
    core_interface
)
add_dependencies(blokecoin_clientversion generate_build_info)

add_subdirectory(crypto)
add_subdirectory(util)
if(ENABLE_IPC)
  add_subdirectory(ipc)
endif()

add_library(blokecoin_consensus STATIC EXCLUDE_FROM_ALL
  arith_uint256.cpp
  consensus/merkle.cpp
  consensus/tx_check.cpp
  hash.cpp
  primitives/block.cpp
  primitives/transaction.cpp
  pubkey.cpp
  script/interpreter.cpp
  script/script.cpp
  script/script_error.cpp
  uint256.cpp
)
target_link_libraries(blokecoin_consensus
  PRIVATE
    core_interface
    blokecoin_crypto
    secp256k1
)

if(WITH_ZMQ)
  add_subdirectory(zmq)
endif()

# Home for common functionality shared by different executables and libraries.
# Similar to `blokecoin_util` library, but higher-level.
add_library(blokecoin_common STATIC EXCLUDE_FROM_ALL
  addresstype.cpp
  base58.cpp
  bech32.cpp
  chain.cpp
  chainparams.cpp
  chainparamsbase.cpp
  coins.cpp
  common/args.cpp
  common/bloom.cpp
  common/config.cpp
  common/init.cpp
  common/interfaces.cpp
  common/messages.cpp
  common/netif.cpp
  common/pcp.cpp
  common/run_command.cpp
  common/settings.cpp
  common/signmessage.cpp
  common/system.cpp
  common/url.cpp
  compressor.cpp
  core_read.cpp
  core_write.cpp
  deploymentinfo.cpp
  external_signer.cpp
  init/common.cpp
  kernel/chainparams.cpp
  key.cpp
  key_io.cpp
  merkleblock.cpp
  musig.cpp
  net_permissions.cpp
  net_types.cpp
  netaddress.cpp
  netbase.cpp
  outputtype.cpp
  policy/feerate.cpp
  policy/policy.cpp
  pow.cpp
  protocol.cpp
  psbt.cpp
  rpc/rawtransaction_util.cpp
  rpc/request.cpp
  rpc/util.cpp
  scheduler.cpp
  script/descriptor.cpp
  script/miniscript.cpp
  script/parsing.cpp
  script/sign.cpp
  script/signingprovider.cpp
  script/solver.cpp
)
target_link_libraries(blokecoin_common
  PRIVATE
    core_interface
    blokecoin_consensus
    blokecoin_util
    univalue
    secp256k1
    Boost::headers
    $<TARGET_NAME_IF_EXISTS:USDT::headers>
    $<$<PLATFORM_ID:Windows>:ws2_32>
)

include(InstallBinaryComponent)

if(ENABLE_WALLET)
  add_subdirectory(wallet)

  if(BUILD_WALLET_TOOL)
    add_executable(blokecoin-wallet
      blokecoin-wallet.cpp
      init/blokecoin-wallet.cpp
      wallet/wallettool.cpp
    )
    add_windows_resources(blokecoin-wallet blokecoin-wallet-res.rc)
    add_windows_application_manifest(blokecoin-wallet)
    target_link_libraries(blokecoin-wallet
      core_interface
      blokecoin_wallet
      blokecoin_common
      blokecoin_util
      Boost::headers
    )
    install_binary_component(blokecoin-wallet HAS_MANPAGE)
  endif()
endif()


# P2P and RPC server functionality used by `blokecoind` and `blokecoin-qt` executables.
add_library(blokecoin_node STATIC EXCLUDE_FROM_ALL
  addrdb.cpp
  addrman.cpp
  banman.cpp
  bip324.cpp
  blockencodings.cpp
  blockfilter.cpp
  consensus/tx_verify.cpp
  dbwrapper.cpp
  deploymentstatus.cpp
  flatfile.cpp
  headerssync.cpp
  httprpc.cpp
  httpserver.cpp
  i2p.cpp
  index/base.cpp
  index/blockfilterindex.cpp
  index/coinstatsindex.cpp
  index/txindex.cpp
  init.cpp
  kernel/chain.cpp
  kernel/checks.cpp
  kernel/coinstats.cpp
  kernel/context.cpp
  kernel/cs_main.cpp
  kernel/disconnected_transactions.cpp
  kernel/mempool_removal_reason.cpp
  mapport.cpp
  net.cpp
  net_processing.cpp
  netgroup.cpp
  node/abort.cpp
  node/blockmanager_args.cpp
  node/blockstorage.cpp
  node/caches.cpp
  node/chainstate.cpp
  node/chainstatemanager_args.cpp
  node/coin.cpp
  node/coins_view_args.cpp
  node/connection_types.cpp
  node/context.cpp
  node/database_args.cpp
  node/eviction.cpp
  node/interface_ui.cpp
  node/interfaces.cpp
  node/kernel_notifications.cpp
  node/mempool_args.cpp
  node/mempool_persist.cpp
  node/mempool_persist_args.cpp
  node/miner.cpp
  node/mini_miner.cpp
  node/minisketchwrapper.cpp
  node/peerman_args.cpp
  node/psbt.cpp
  node/timeoffsets.cpp
  node/transaction.cpp
  node/txdownloadman_impl.cpp
  node/txorphanage.cpp
  node/txreconciliation.cpp
  node/utxo_snapshot.cpp
  node/warnings.cpp
  noui.cpp
  policy/ephemeral_policy.cpp
  policy/fees/block_policy_estimator.cpp
  policy/fees/block_policy_estimator_args.cpp
  policy/packages.cpp
  policy/rbf.cpp
  policy/settings.cpp
  policy/truc_policy.cpp
  rest.cpp
  rpc/blokechain.cpp
  rpc/external_signer.cpp
  rpc/fees.cpp
  rpc/mempool.cpp
  rpc/mining.cpp
  rpc/net.cpp
  rpc/node.cpp
  rpc/output_script.cpp
  rpc/rawtransaction.cpp
  rpc/server.cpp
  rpc/server_util.cpp
  rpc/signmessage.cpp
  rpc/txoutproof.cpp
  script/sigcache.cpp
  signet.cpp
  torcontrol.cpp
  txdb.cpp
  txgraph.cpp
  txmempool.cpp
  txrequest.cpp
  validation.cpp
  validationinterface.cpp
  versionbits.cpp
  $<$<TARGET_EXISTS:blokecoin_wallet>:wallet/init.cpp>
  $<$<NOT:$<TARGET_EXISTS:blokecoin_wallet>>:dummywallet.cpp>
)
target_link_libraries(blokecoin_node
  PRIVATE
    core_interface
    blokecoin_common
    blokecoin_util
    $<TARGET_NAME_IF_EXISTS:blokecoin_zmq>
    leveldb
    minisketch
    univalue
    Boost::headers
    $<TARGET_NAME_IF_EXISTS:libevent::core>
    $<TARGET_NAME_IF_EXISTS:libevent::extra>
    $<TARGET_NAME_IF_EXISTS:libevent::pthreads>
    $<TARGET_NAME_IF_EXISTS:USDT::headers>
)

# Blokecoin wrapper executable that can call other executables.
if(BUILD_BITCOIN_BIN)
  add_executable(blokecoin blokecoin.cpp)
  add_windows_resources(blokecoin blokecoin-res.rc)
  add_windows_application_manifest(blokecoin)
  target_link_libraries(blokecoin core_interface blokecoin_common blokecoin_util)
  install_binary_component(blokecoin HAS_MANPAGE)
endif()

# Blokecoin Core blokecoind.
if(BUILD_DAEMON)
  add_executable(blokecoind
    blokecoind.cpp
    init/blokecoind.cpp
  )
  add_windows_resources(blokecoind blokecoind-res.rc)
  add_windows_application_manifest(blokecoind)
  target_link_libraries(blokecoind
    core_interface
    blokecoin_node
    $<TARGET_NAME_IF_EXISTS:blokecoin_wallet>
  )
  install_binary_component(blokecoind HAS_MANPAGE)
endif()
if(ENABLE_IPC AND BUILD_DAEMON)
  add_executable(blokecoin-node
    blokecoind.cpp
    init/blokecoin-node.cpp
  )
  target_link_libraries(blokecoin-node
    core_interface
    blokecoin_node
    blokecoin_ipc
    $<TARGET_NAME_IF_EXISTS:blokecoin_wallet>
  )
  install_binary_component(blokecoin-node INTERNAL)
endif()

if(ENABLE_IPC AND BUILD_TESTS)
    # blokecoin_ipc_test library target is defined here in src/CMakeLists.txt
    # instead of src/test/CMakeLists.txt so capnp files in src/test/ are able to
    # reference capnp files in src/ipc/capnp/ by relative path. The Cap'n Proto
    # compiler only allows importing by relative path when the importing and
    # imported files are underneath the same compilation source prefix, so the
    # source prefix must be src/, not src/test/
    add_library(blokecoin_ipc_test STATIC EXCLUDE_FROM_ALL
      test/ipc_test.cpp
    )
    target_capnp_sources(blokecoin_ipc_test ${PROJECT_SOURCE_DIR}
      test/ipc_test.capnp
    )
    add_dependencies(blokecoin_ipc_test blokecoin_ipc_headers)
endif()


add_library(blokecoin_cli STATIC EXCLUDE_FROM_ALL
  compat/stdin.cpp
  rpc/client.cpp
)
target_link_libraries(blokecoin_cli
  PUBLIC
    core_interface
    univalue
)


# Blokecoin Core RPC client
if(BUILD_CLI)
  add_executable(blokecoin-cli blokecoin-cli.cpp)
  add_windows_resources(blokecoin-cli blokecoin-cli-res.rc)
  add_windows_application_manifest(blokecoin-cli)
  target_link_libraries(blokecoin-cli
    core_interface
    blokecoin_cli
    blokecoin_common
    blokecoin_util
    libevent::core
    libevent::extra
  )
  install_binary_component(blokecoin-cli HAS_MANPAGE)
endif()


if(BUILD_TX)
  add_executable(blokecoin-tx blokecoin-tx.cpp)
  add_windows_resources(blokecoin-tx blokecoin-tx-res.rc)
  add_windows_application_manifest(blokecoin-tx)
  target_link_libraries(blokecoin-tx
    core_interface
    blokecoin_common
    blokecoin_util
    univalue
  )
  install_binary_component(blokecoin-tx HAS_MANPAGE)
endif()


if(BUILD_UTIL)
  add_executable(blokecoin-util blokecoin-util.cpp)
  add_windows_resources(blokecoin-util blokecoin-util-res.rc)
  add_windows_application_manifest(blokecoin-util)
  target_link_libraries(blokecoin-util
    core_interface
    blokecoin_common
    blokecoin_util
  )
  install_binary_component(blokecoin-util HAS_MANPAGE)
endif()


if(BUILD_GUI)
  add_subdirectory(qt)
endif()


if(BUILD_KERNEL_LIB)
  add_subdirectory(kernel)
  if (BUILD_KERNEL_TEST)
    add_subdirectory(test/kernel)
  endif()
  if(BUILD_UTIL_CHAINSTATE)
    add_executable(blokecoin-chainstate
      blokecoin-chainstate.cpp
    )
    add_windows_application_manifest(blokecoin-chainstate)
    target_link_libraries(blokecoin-chainstate
      PRIVATE
        core_interface
        blokecoinkernel
    )
    install_binary_component(blokecoin-chainstate INTERNAL)
  endif()
endif()


add_subdirectory(test/util)
if(BUILD_BENCH)
  add_subdirectory(bench)
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

if(BUILD_FUZZ_BINARY)
  add_subdirectory(test/fuzz)
endif()
