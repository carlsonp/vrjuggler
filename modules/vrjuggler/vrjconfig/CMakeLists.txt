
set(CMAKE_JAVA_INCLUDE_PATH ${JCCL_JAVA_CLASSPATH})

set(COMMONEDITOR "${CMAKE_CURRENT_SOURCE_DIR}/commoneditors")

get_filename_component(EXTERNALS "${CMAKE_CURRENT_SOURCE_DIR}/../../../external" ABSOLUTE)

find_jar(JGRAPH_JAR
	jgraph
	PATHS
	${EXTERNALS}/jgraph/lib/)

find_jar(JGRAPHADDONS_JAR
	jgraphaddons
	PATHS
	${EXTERNALS}/jgraph/lib/)

set(CMAKE_JAVA_INCLUDE_PATH ${JCCL_JAVA_CLASSPATH} "${JGRAPH_JAR}" "${JGRAPHADDONS_JAR}")

if(JOGL_FOUND)
	# TODO Always false right now, and must migrate net.java.games.jogl.* to javax.media.opengl.*
	set(COMMONEDITORS_JOGL 
		"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/Ascension.java"
		"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/Intersense.java"
		"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/MovableObject.java"
		"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/PositionalDeviceEditor.java"
		"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/Texture.java")
endif()

add_jar(CommonEditors
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ConfigPtrComboBoxModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ConfigPtrComboBoxRenderer.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ConfigPtrListModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/CoordinateFrameEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/CustomEditorDialog.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DefaultProxyTypeEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DeviceConfig.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DeviceEditorPanel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/AbstractCustomVertexRenderer.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/AbstractDeviceUnitPropertyHandler.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/BaseDeviceInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ConfigElementHolder.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DefaultDeviceGraphCellCreator.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DefaultUnitPropertyHandler.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DeviceGraphCellCreator.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DeviceGraphLayoutCache.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DeviceGraphModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DeviceInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/DeviceUnitPropertyHandler.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/extras/IntersenseVertexView.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/extras/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/extras/SimAnalogUnitPropertyHandler.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/GraphHelpers.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DeviceGraph.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/MultiUnitDeviceVertexView.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/NoSuchPortException.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/PortComponent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ProxiedDeviceCellViewFactory.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ProxyInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ProxyPointerInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ProxyToDeviceEdge.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/ProxyVertexView.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/RelativeDeviceInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/RelativeDeviceToProxyEdge.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/UnitConstants.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/UnitInfo.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/UnitLabel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/devicegraph/UnitTypeHelpers.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DeviceProxyGraphEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/DeviceTypeEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/EditorConstants.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/EditorHelpers.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/DeviceUnitEvent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/DeviceUnitListener.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/TransformChangeEvent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/TransformChangeListener.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/UnitInfoEvent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/event/UnitInfoListener.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/HashMultiMap.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Add16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/add.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Arrow16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis00.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis01.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis02.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis03.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis04.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis05.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis06.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis07.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis08.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis09.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis10.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis11.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis12.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis13.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis14.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis15.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis16.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis17.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis18.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis19.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis20.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis21.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis22.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/axis23.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/carpet.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/ClusterIcon.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Cut16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Delete16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Disconnect16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Edit16.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/edit_disabled.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/edit.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/ert2.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/New16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Paste16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Ports16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/position16.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Preferences16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Refresh16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Remove16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/remove_disabled.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/remove.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/Select16.gif"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/images/transmitter16.png"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/KeyboardEditorPanel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/AbstractPlacerModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/Placer.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerModelEvent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerModelListener.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerRenderer.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerSelectionEvent.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/placer/PlacerSelectionListener.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/PositionProxyEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ProxyEditorUI.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ProxyTreeEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/proxytree/package.html"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/proxytree/ProxyChooser.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/proxytree/ProxyTree.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/proxytree/ProxyTreeModel.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/ProxyTypeEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/SerialPortChooser.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/SimpleEditorDialog.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/SimpleProxyEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/TransformTranslationEditor.java"
	"${COMMONEDITOR}/org/vrjuggler/vrjconfig/commoneditors/TransmitterTransformPanel.java"
	${COMMONEDITORS_JOGL})

set_target_properties(CommonEditors PROPERTIES FOLDER "VRJConfig Java")

get_property(CommmonEditorsJar TARGET CommonEditors PROPERTY JAR_FILE)
list(APPEND CMAKE_JAVA_INCLUDE_PATH ${CommmonEditorsJar})

add_jar(VRJConfig
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ConfigDefinitionEditor.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/AbstractControlPanelModel.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/CategoryNode.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanel.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelLayout.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelModel.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelNode.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelToolbar.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelView.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/ControlPanelViewModel.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/DefaultControlPanelModel.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/EditorNode.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/io/ControlPanelIOConstants.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/io/ControlPanelParser.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/io/ControlPanelReader.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/io/MissingAttributeException.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/io/ParseException.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/TypeNode.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/WatermarkImage.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/controlpanel/WizardNode.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Add16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/analog_devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/analog_proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/audio64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Back16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/cluster64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/ContextEdit16.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Copy16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Cut16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/digital_devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/digital_proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/display64.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/display64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Forward16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/glove_devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/glove_proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/keyboard_mouse_devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/keyboard_mouse_proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/movedown.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/moveup.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/newchunk.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/new_cluster_wizard64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/newdesc.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/new_device_wizard64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/new.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/newinclude.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/OpenAdd16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/open.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/Paste16.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/popdown.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/positional_devices64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/positional_proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/proxies64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/redo.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/removeinclude.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/saveall.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/saveas.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/save.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/simulator64.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/undo.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/vrjuggler.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/images/watermark_logo.png"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/PopupButton.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/ConfigFileFilter.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/ConfigFileView.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/ConfigToolbar.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/ConnectionDialog.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/ContextToolbar.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/EditContextPopup.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/images/add24.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/images/remove24.gif"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/ui/NewConfigDialog.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/VrjConfigConstants.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/org/vrjuggler/vrjconfig/VrjConfig.java"
	"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-java-laf.txt")
add_dependencies(VRJConfig jccl_java CommonEditors)

set_target_properties(VRJConfig PROPERTIES FOLDER "VRJConfig Java")

# Set up custom command to copy finished jars to a more suitable place
get_property(JAR TARGET CommonEditors PROPERTY JAR_FILE)
vrjugglersuite_copy_and_install(VRJCONFIG_COPIED_FILES
	"${VRJ_SHARE_DIR}/java"
	vrjconfig
	"${JAR}"
	"${JGRAPH_JAR}" # TODO exclude system versions
	"${JGRAPHADDONS_JAR}") # TODO exclude system versions

get_property(JAR TARGET VRJConfig PROPERTY JAR_FILE)
vrjugglersuite_copy_and_install(VRJCONFIG_COPIED_FILES
	"${VRJ_SHARE_DIR}/beans"
	vrjconfig
	"${JAR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/VRJConfig.xml")

add_custom_target(VRJConfig_overall ALL DEPENDS ${VRJCONFIG_COPIED_FILES})
