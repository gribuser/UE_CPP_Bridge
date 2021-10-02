# UE_CPP_Bridge Unreal C++ plugin
Various utility foe UE C++ projects, intended to build the same code both IN and OUT of UE.

  Like you can use UE_CPP_Bridge::TBridgeArray<int> instead of TArray<int>, and while built with
  `UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE = 1` this class is 100% UE-compatible. You can use it with
  Blueprints, etc (actually, it IS normal TArray). But you can build and use the very same
  code in any non-UE project. Just switch
  `UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE = 0` and you can include your headers and build your code in
  linux daemon oк whatever.

  Not really polished at the current stage, but as far as I need this stuff working it will (eventually)
  get to the really nice shape.
