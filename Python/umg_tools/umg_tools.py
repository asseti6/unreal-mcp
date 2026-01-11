"""
UMG Tools for Unreal MCP.

This module provides tools for creating and manipulating UMG Widget Blueprints in Unreal Engine.
"""

import logging
from typing import Any, Dict, List, Union
from mcp.server.fastmcp import FastMCP, Context
from utils.widgets.widget_components import (
    create_widget_blueprint as create_widget_blueprint_impl,
    bind_widget_component_event as bind_widget_component_event_impl,
    add_child_widget_component_to_parent as add_child_widget_component_to_parent_impl,
    create_parent_and_child_widget_components as create_parent_and_child_widget_components_impl,
    set_widget_component_placement as set_widget_component_placement_impl,
    add_widget_component_to_widget as add_widget_component_to_widget_impl,
    set_widget_component_property as set_widget_component_property_impl,
    set_text_block_widget_component_binding as set_text_block_widget_component_binding_impl,
    get_widget_blueprint_metadata_impl,
    create_widget_input_handler as create_widget_input_handler_impl,
    remove_widget_function_graph as remove_widget_function_graph_impl,
    reorder_widget_children as reorder_widget_children_impl,
    # Widget configuration functions
    set_widget_design_size_mode as set_widget_design_size_mode_impl,
    set_widget_parent_class as set_widget_parent_class_impl
)
from utils.widgets.widget_screenshot import (
    capture_widget_screenshot_impl
)
from utils.widgets.widget_animations import (
    create_widget_animation as create_widget_animation_impl,
    add_animation_track as add_animation_track_impl,
    add_animation_keyframe as add_animation_keyframe_impl,
    set_animation_properties as set_animation_properties_impl,
    get_widget_animations as get_widget_animations_impl,
    delete_widget_animation as delete_widget_animation_impl
)

# Get logger
logger = logging.getLogger("UnrealMCP")

def register_umg_tools(mcp: FastMCP):
    """Register UMG tools with the MCP server."""

    @mcp.tool()
    def create_umg_widget_blueprint(
        ctx: Context,
        widget_name: str,
        parent_class: str = "UserWidget",
        path: str = "/Game/Widgets"
    ) -> Dict[str, object]:
        """
        Create a new UMG Widget Blueprint.
        
        Args:
            widget_name: Name of the widget blueprint to create
            parent_class: Parent class for the widget (default: UserWidget)
            path: Content browser path where the widget should be created
            
        Returns:
            Dict containing success status and widget path
            
        Examples:
            # Create a basic UserWidget blueprint
            create_umg_widget_blueprint(widget_name="MyWidget")
            
            # Create a widget with custom parent class
            create_umg_widget_blueprint(widget_name="MyCustomWidget", parent_class="MyBaseUserWidget")
            
            # Create a widget in a custom folder
            create_umg_widget_blueprint(widget_name="MyWidget", path="/Game/UI/Widgets")
        """
        # Call aliased implementation
        return create_widget_blueprint_impl(ctx, widget_name, parent_class, path)

    @mcp.tool()
    def bind_widget_component_event(
        ctx: Context,
        widget_name: str,
        widget_component_name: str,
        event_name: str,
        function_name: str = ""
    ) -> Dict[str, object]:
        """
        Bind an event on a widget component to a function.
        
        Args:
            widget_name: Name of the target Widget Blueprint
            widget_component_name: Name of the widget component (button, etc.)
            event_name: Name of the event to bind (OnClicked, etc.)
            function_name: Name of the function to create/bind to (defaults to f"{widget_component_name}_{event_name}")
            
        Returns:
            Dict containing success status and binding information
            
        Examples:
            # Bind button click event
            bind_widget_component_event(
                widget_name="LoginScreen",
                widget_component_name="LoginButton",
                event_name="OnClicked"
            )
            
            # Bind with custom function name
            bind_widget_component_event(
                widget_name="MainMenu",
                widget_component_name="QuitButton",
                event_name="OnClicked",
                function_name="ExitApplication"
            )
        """
        # Call aliased implementation
        return bind_widget_component_event_impl(ctx, widget_name, widget_component_name, event_name, function_name)

    @mcp.tool()
    def set_text_block_widget_component_binding(
        ctx: Context,
        widget_name: str,
        text_block_name: str,
        binding_property: str,
        binding_type: str = "Text",
        variable_type: str = "Text"
    ) -> Dict[str, object]:
        """
        Set up a property binding for a Text Block widget.
        
        Args:
            widget_name: Name of the target Widget Blueprint
            text_block_name: Name of the Text Block to bind
            binding_property: Name of the property to bind to
            binding_type: Type of binding (Text, Visibility, etc.)
            variable_type: Type of variable to create if it doesn't exist (Text, String, Int, Float, Bool)
            
        Returns:
            Dict containing success status and binding information
            
        Examples:
            # Set basic text binding
            set_text_block_widget_component_binding(
                widget_name="PlayerHUD",
                text_block_name="ScoreText",
                binding_property="CurrentScore"
            )
            
            # Set binding with specific binding type and variable type
            set_text_block_widget_component_binding(
                widget_name="GameUI",
                text_block_name="TimerText",
                binding_property="RemainingTime",
                binding_type="Text",
                variable_type="Int"
            )
            
            # Set binding with custom variable type
            set_text_block_widget_component_binding(
                widget_name="ShopUI",
                text_block_name="PriceText",
                binding_property="ItemPrice",
                binding_type="Text",
                variable_type="Float"
            )
        """
        # Call aliased implementation
        return set_text_block_widget_component_binding_impl(ctx, widget_name, text_block_name, binding_property, binding_type, variable_type)

    @mcp.tool()
    def add_child_widget_component_to_parent(
        ctx: Context,
        widget_name: str,
        parent_component_name: str,
        child_component_name: str,
        create_parent_if_missing: bool = False,
        parent_component_type: str = "Border",
        parent_position: List[float] = [0.0, 0.0],
        parent_size: List[float] = [300.0, 200.0]
    ) -> Dict[str, object]:
        """
        Add a widget component as a child to another component.
        
        This function can:
        1. Add an existing component inside an existing component
        2. If the parent doesn't exist, optionally create it and add the child inside
        
        Args:
            widget_name: Name of the target Widget Blueprint
            parent_component_name: Name of the parent component
            child_component_name: Name of the child component to add to the parent
            create_parent_if_missing: Whether to create the parent component if it doesn't exist
            parent_component_type: Type of parent component to create if needed (e.g., "Border", "VerticalBox")
            parent_position: [X, Y] position of the parent component if created
            parent_size: [Width, Height] of the parent component if created
            
        Returns:
            Dict containing success status and component relationship information
            
        Examples:
            # Add an existing text block inside an existing border
            add_child_widget_component_to_parent(
                widget_name="MyWidget",
                parent_component_name="ContentBorder",
                child_component_name="HeaderText"
            )
            
            # Add an existing button to a new vertical box (creates if missing)
            add_child_widget_component_to_parent(
                widget_name="GameMenu",
                parent_component_name="ButtonsContainer",
                child_component_name="StartButton",
                create_parent_if_missing=True,
                parent_component_type="VerticalBox",
                parent_position=[100.0, 100.0],
                parent_size=[300.0, 400.0]
            )
        """
        # Call aliased implementation
        return add_child_widget_component_to_parent_impl(
            ctx, 
            widget_name, 
            parent_component_name, 
            child_component_name, 
            create_parent_if_missing, 
            parent_component_type, 
            parent_position, 
            parent_size
        )

    @mcp.tool()
    def create_parent_and_child_widget_components(
        ctx: Context,
        widget_name: str,
        parent_component_name: str,
        child_component_name: str,
        parent_component_type: str = "Border",
        child_component_type: str = "TextBlock",
        parent_position: List[float] = [0.0, 0.0],
        parent_size: List[float] = [300.0, 200.0],
        child_attributes: Dict[str, object] = None
    ) -> Dict[str, object]:
        """
        Create a new parent widget component with a new child component.
        
        This function creates both components from scratch:
        1. Creates the parent component
        2. Creates the child component
        3. Adds the child inside the parent
        Note: This function creates exactly one parent and one child component.
              It cannot be used to create multiple nested levels in a single call.

        Args:
            widget_name: Name of the target Widget Blueprint
            parent_component_name: Name for the new parent component
            child_component_name: Name for the new child component
            parent_component_type: Type of parent component to create (e.g., "Border", "VerticalBox")
            child_component_type: Type of child component to create (e.g., "TextBlock", "Button")
            parent_position: [X, Y] position of the parent component
            parent_size: [Width, Height] of the parent component
            child_attributes: Additional attributes for the child component (content, colors, etc.)
            
        Returns:
            Dict containing success status and component creation information
            
        Examples:
            # Create a border with a text block inside
            create_parent_and_child_widget_components(
                widget_name="MyWidget",
                parent_component_name="HeaderBorder",
                child_component_name="TitleText",
                parent_component_type="Border",
                child_component_type="TextBlock",
                parent_position=[50.0, 50.0],
                parent_size=[400.0, 100.0],
                child_attributes={"text": "Welcome to My Game", "font_size": 24}
            )
            
            # Create a scroll box with a vertical box inside
            create_parent_and_child_widget_components(
                widget_name="InventoryScreen",
                parent_component_name="ItemsScrollBox",
                child_component_name="ItemsContainer",
                parent_component_type="ScrollBox",
                child_component_type="VerticalBox",
                parent_position=[100.0, 200.0],
                parent_size=[300.0, 400.0]
            )
        """
        # Call aliased implementation
        return create_parent_and_child_widget_components_impl(
            ctx, 
            widget_name, 
            parent_component_name, 
            child_component_name, 
            parent_component_type, 
            child_component_type, 
            parent_position, 
            parent_size,
            child_attributes
        )

    @mcp.tool()
    def set_widget_component_placement(
        ctx: Context,
        widget_name: str,
        component_name: str,
        position: List[float] = None,
        size: List[float] = None,
        alignment: List[float] = None
    ) -> Dict[str, object]:
        """
        Change the placement (position/size) of a widget component.
        
        Args:
            widget_name: Name of the target Widget Blueprint
            component_name: Name of the component to modify
            position: Optional [X, Y] new position in the canvas panel
            size: Optional [Width, Height] new size for the component
            alignment: Optional [X, Y] alignment values (0.0 to 1.0)
            
        Returns:
            Dict containing success status and updated placement information
            
        Examples:
            # Change just the position of a component
            set_widget_component_placement(
                widget_name="MainMenu",
                component_name="TitleText",
                position=[350.0, 75.0]
            )
            
            # Change both position and size
            set_widget_component_placement(
                widget_name="HUD",
                component_name="HealthBar",
                position=[50.0, 25.0],
                size=[250.0, 30.0]
            )
            
            # Change alignment (center-align)
            set_widget_component_placement(
                widget_name="Inventory",
                component_name="ItemName",
                alignment=[0.5, 0.5]
            )
        """
        # Call aliased implementation
        return set_widget_component_placement_impl(ctx, widget_name, component_name, position, size, alignment)

    @mcp.tool()
    def add_widget_component_to_widget(
        ctx: Context,
        widget_name: str,
        component_name: str,
        component_type: str,
        position: List[float] = None,
        size: List[float] = None,
        **kwargs
    ) -> Dict[str, object]:
        """
        Unified function to add any type of widget component to a UMG Widget Blueprint.
        
        Args:
            widget_name: Name of the target Widget Blueprint
            component_name: Name to give the new component
            component_type: Type of component to add (e.g., "TextBlock", "Button", etc.)
            position: Optional [X, Y] position in the canvas panel
            size: Optional [Width, Height] of the component
            **kwargs: Additional parameters specific to the component type
                For Border components:
                - background_color/brush_color: [R, G, B, A] color values (0.0-1.0)
                  Note: To achieve transparent backgrounds, set the Alpha value (A) in the color array
                - opacity: Value between 0.0-1.0 setting the render opacity of the entire border
                  and its contents. This is separate from the brush color's alpha.
                - use_brush_transparency: Boolean (True/False) to enable the "Use Brush Transparency" option
                  Required for alpha transparency to work properly with rounded corners or other complex brushes
                - padding: [Left, Top, Right, Bottom] values
            
        Returns:
            Dict containing success status and component properties
            
        Examples:
            # Add a text block
            add_widget_component_to_widget(
                widget_name="MyWidget",
                component_name="HeaderText",
                component_type="TextBlock",
                position=[100, 50],
                size=[200, 50],
                text="Hello World",
                font_size=24
            )
            
            # Add a button
            add_widget_component_to_widget(
                widget_name="MyWidget",
                component_name="SubmitButton",
                component_type="Button",
                position=[100, 100],
                size=[200, 50],
                text="Submit",
                background_color=[0.2, 0.4, 0.8, 1.0]
            )
        """
        # Call aliased implementation
        return add_widget_component_to_widget_impl(ctx, widget_name, component_name, component_type, position, size, **kwargs)

    @mcp.tool()
    def set_widget_component_property(
        ctx: Context,
        widget_name: str,
        component_name: str,
        **kwargs
    ) -> Dict[str, object]:
        """
        Set one or more properties on a specific component within a UMG Widget Blueprint.

        Parameters:
            widget_name: Name of the target Widget Blueprint
            component_name: Name of the component to modify
            kwargs: Properties to set (as keyword arguments or a dict)

        Examples:
            
            # Set the text and color of a TextBlock, including a struct property (ColorAndOpacity)
            set_widget_component_property(
                ctx,
                "MyWidget",
                "MyTextBlock",
                Text="Red Text",
                ColorAndOpacity={
                    "SpecifiedColor": {
                        "R": 1.0,
                        "G": 0.0,
                        "B": 0.0,
                        "A": 1.0
                    }
                }
            )
            # Simple properties can be passed directly; struct properties (like ColorAndOpacity) as dicts.

            # Set the brush color (including opacity) of a Border using a flat RGBA dictionary
            set_widget_component_property(
                ctx,
                "MyWidget",
                "MyBorder",
                BrushColor={
                    "R": 1.0,
                    "G": 1.0,
                    "B": 1.0,
                    "A": 0.3
                }
            )
        """ 
        logger.info(f"[DEBUG] TOOL ENTRY: set_widget_component_property called with widget_name={widget_name}, component_name={component_name}, kwargs={kwargs}")
        try:
            # Call aliased implementation
            return set_widget_component_property_impl(ctx, widget_name, component_name, **(kwargs if isinstance(kwargs, dict) else {"Text": kwargs}))
        except Exception as e:
            logger.error(f"[ERROR] Exception in set_widget_component_property: {e}")
            raise

    @mcp.tool()
    def get_widget_blueprint_metadata(
        ctx: Context,
        widget_name: str,
        fields: List[str] = None,
        container_name: str = "CanvasPanel_0"
    ) -> Dict[str, Any]:
        """
        Get comprehensive metadata about a Widget Blueprint.

        This is the consolidated metadata tool that replaces:
        - check_widget_component_exists → fields=["components"] then check component name
        - get_widget_component_layout → fields=["layout"]
        - get_widget_container_component_dimensions → fields=["dimensions"]

        Args:
            widget_name: Name of the target Widget Blueprint (e.g., "WBP_MainMenu", "/Game/UI/MyWidget")
            fields: List of fields to include. If not specified, returns all fields. Options:
                - "components" - List all widget components with basic info (name, type, is_variable, is_visible)
                - "layout" - Full hierarchical layout with positions, sizes, anchors, slot properties
                - "dimensions" - Container dimensions (width, height)
                - "hierarchy" - Simplified parent/child widget tree (names and types only)
                - "bindings" - Property bindings for dynamic content
                - "events" - Bound events and delegates
                - "variables" - Blueprint variables
                - "functions" - Blueprint functions with inputs/outputs
                - "*" - Return all fields (default)
            container_name: Container name for dimensions field (default: "CanvasPanel_0")

        Returns:
            Dict containing:
                - success (bool): True if the operation succeeded
                - widget_name (str): Name of the widget blueprint
                - asset_path (str): Full asset path
                - parent_class (str): Parent class name
                - components (dict): Component list with count (if requested)
                - layout (dict): Hierarchical layout info (if requested)
                - dimensions (dict): Container dimensions (if requested)
                - hierarchy (dict): Simple widget tree (if requested)
                - bindings (dict): Property bindings (if requested)
                - events (dict): Event bindings (if requested)
                - variables (dict): Blueprint variables (if requested)
                - functions (dict): Blueprint functions (if requested)
                - error (str): Error message if failed

        Examples:
            # Get all metadata
            metadata = get_widget_blueprint_metadata(widget_name="WBP_MainMenu")

            # Check if a component exists (replaces check_widget_component_exists)
            metadata = get_widget_blueprint_metadata(
                widget_name="WBP_MainMenu",
                fields=["components"]
            )
            exists = any(c["name"] == "HeaderText" for c in metadata.get("components", {}).get("components", []))

            # Get layout info (replaces get_widget_component_layout)
            metadata = get_widget_blueprint_metadata(
                widget_name="WBP_MainMenu",
                fields=["layout"]
            )
            layout = metadata.get("layout", {})

            # Get container dimensions (replaces get_widget_container_component_dimensions)
            metadata = get_widget_blueprint_metadata(
                widget_name="WBP_MainMenu",
                fields=["dimensions"],
                container_name="CanvasPanel_0"
            )
            width = metadata.get("dimensions", {}).get("width", 1920)
            height = metadata.get("dimensions", {}).get("height", 1080)

            # Get multiple specific fields
            metadata = get_widget_blueprint_metadata(
                widget_name="WBP_MainMenu",
                fields=["components", "variables", "bindings"]
            )
        """
        return get_widget_blueprint_metadata_impl(ctx, widget_name, fields, container_name)

    @mcp.tool()
    def capture_widget_screenshot(
        ctx: Context,
        widget_name: str,
        width: int = 800,
        height: int = 600,
        format: str = "png"
    ) -> Dict[str, Any]:
        """
        Capture a screenshot of a UMG Widget Blueprint preview.

        This renders the widget to an image and returns it as base64-encoded data that AI can view.
        This is the BEST way for AI to understand widget layout visually.

        Args:
            widget_name: Name of the widget blueprint to capture
            width: Screenshot width in pixels (default: 800, range: 1-8192)
            height: Screenshot height in pixels (default: 600, range: 1-8192)
            format: Image format - "png" (default) or "jpg"

        Returns:
            Dict containing:
                - success: Whether the screenshot was captured
                - image_base64: Base64-encoded image data (viewable by AI)
                - width: Actual image width
                - height: Actual image height
                - format: Image format used
                - image_size_bytes: Size of the compressed image
                - message: Success or error message

        Examples:
            # Capture a widget screenshot
            result = capture_widget_screenshot(widget_name="WBP_MainMenu")
            if result["success"]:
                print(f"Screenshot captured: {result['width']}x{result['height']}, "
                      f"{result['image_size_bytes']} bytes")
                # AI can now view the widget layout visually

            # Capture at higher resolution
            result = capture_widget_screenshot(
                widget_name="WBP_LoginScreen",
                width=1920,
                height=1080
            )

            # Capture as JPEG (smaller file size)
            result = capture_widget_screenshot(
                widget_name="WBP_Settings",
                width=1024,
                height=768,
                format="jpg"
            )
        """
        return capture_widget_screenshot_impl(ctx, widget_name, width, height, format)

    @mcp.tool()
    def create_widget_input_handler(
        ctx: Context,
        widget_name: str,
        input_type: str,
        input_event: str,
        trigger: str = "Pressed",
        handler_name: str = "",
        component_name: str = ""
    ) -> Dict[str, object]:
        """
        Create an input event handler in a Widget Blueprint.

        This creates handlers for input events that are NOT exposed as standard widget
        delegates (like OnClicked). Use this for handling:
        - Right mouse button clicks (for context menus)
        - Middle mouse button (for panning/special actions)
        - Keyboard shortcuts on focusable widgets
        - Touch gestures
        - Drag and drop operations
        - Focus changes

        The tool creates:
        1. A custom event function in the Widget Blueprint's event graph
        2. An override of the appropriate input handler (OnMouseButtonDown, OnKeyDown, etc.)
        3. Logic to detect the specific input and call your custom event

        Args:
            widget_name: Name of the target Widget Blueprint
            input_type: Type of input to handle:
                - "MouseButton" - Mouse button events (left, right, middle, thumb)
                - "Key" - Keyboard key events
                - "Touch" - Touch/gesture events
                - "Focus" - Widget focus events
                - "Drag" - Drag and drop events
            input_event: Specific input event to handle:
                - For MouseButton: "LeftMouseButton", "RightMouseButton", "MiddleMouseButton",
                                   "ThumbMouseButton", "ThumbMouseButton2"
                - For Key: Any key name (e.g., "Enter", "Escape", "SpaceBar", "A", "F1", etc.)
                - For Touch: "Touch", "Pinch", "Swipe"
                - For Focus: "FocusReceived", "FocusLost"
                - For Drag: "DragDetected", "DragEnter", "DragLeave", "DragOver", "Drop"
            trigger: When to trigger the handler (default: "Pressed"):
                - "Pressed" - On button/key press
                - "Released" - On button/key release
                - "DoubleClick" - On double click (mouse only)
            handler_name: Name of the custom event function to create.
                         If empty, auto-generates (e.g., "OnRightMouseButtonPressed")
            component_name: Optional widget component name for component-specific handling.
                           If empty, handles at widget blueprint level.

        Returns:
            Dict containing:
                - success: Whether the handler was created
                - handler_name: The actual name of the created handler function
                - input_type: The input type configured
                - input_event: The specific event that triggers the handler
                - message: Description of what was created

        Examples:
            # Create right-click handler for inventory context menu
            create_widget_input_handler(
                widget_name="WBP_InventorySlot",
                input_type="MouseButton",
                input_event="RightMouseButton",
                handler_name="OnSlotRightClicked"
            )

            # Create Escape key handler to close a menu
            create_widget_input_handler(
                widget_name="WBP_PauseMenu",
                input_type="Key",
                input_event="Escape",
                handler_name="OnCloseMenu"
            )

            # Create middle mouse button handler for map panning
            create_widget_input_handler(
                widget_name="WBP_WorldMap",
                input_type="MouseButton",
                input_event="MiddleMouseButton",
                trigger="Pressed",
                handler_name="OnStartPan"
            )

            # Create double-click handler
            create_widget_input_handler(
                widget_name="WBP_ItemList",
                input_type="MouseButton",
                input_event="LeftMouseButton",
                trigger="DoubleClick",
                handler_name="OnItemDoubleClicked"
            )
        """
        return create_widget_input_handler_impl(
            ctx, widget_name, input_type, input_event, trigger, handler_name, component_name
        )

    @mcp.tool()
    def remove_widget_function_graph(
        ctx: Context,
        widget_name: str,
        function_name: str
    ) -> Dict[str, object]:
        """
        Remove a function graph from a Widget Blueprint.

        Use this to clean up broken/corrupt function graphs that prevent compilation,
        remove unwanted override functions, or reset widget event handlers.

        Args:
            widget_name: Name of the target Widget Blueprint
            function_name: Name of the function graph to remove (e.g., "OnMouseButtonDown")

        Returns:
            Dict containing success status and removal information

        Examples:
            # Remove a broken OnMouseButtonDown override that prevents compilation
            remove_widget_function_graph(
                widget_name="WBP_InventorySlot",
                function_name="OnMouseButtonDown"
            )

            # Remove a custom event handler
            remove_widget_function_graph(
                widget_name="WBP_MainMenu",
                function_name="OnRightClickHandler"
            )

            # Clean up an override function to start fresh
            remove_widget_function_graph(
                widget_name="WBP_InteractiveWidget",
                function_name="OnKeyDown"
            )
        """
        return remove_widget_function_graph_impl(ctx, widget_name, function_name)

    @mcp.tool()
    def reorder_widget_children(
        ctx: Context,
        widget_name: str,
        container_name: str,
        child_order: List[str]
    ) -> Dict[str, object]:
        """
        Reorder children within a container widget (HorizontalBox, VerticalBox, etc.).

        Use this when components are in wrong visual order within a container.
        Children will be arranged in the order specified in child_order list.

        Args:
            widget_name: Name of the target Widget Blueprint
            container_name: Name of the container component (e.g., "ContentBox", "ButtonsContainer")
            child_order: List of child component names in desired left-to-right (or top-to-bottom) order

        Returns:
            Dict containing success status and updated child order

        Examples:
            # Fix order in HorizontalBox: put KeyBadge before PromptText
            reorder_widget_children(
                widget_name="WBP_InteractionIndicator",
                container_name="ContentBox",
                child_order=["KeyBadgeOuter", "PromptText"]
            )

            # Reorder buttons in a VerticalBox
            reorder_widget_children(
                widget_name="WBP_MainMenu",
                container_name="ButtonsContainer",
                child_order=["PlayButton", "SettingsButton", "QuitButton"]
            )
        """
        return reorder_widget_children_impl(ctx, widget_name, container_name, child_order)

    # ============================================================================
    # Widget Configuration Tools
    # Tools for configuring widget blueprint settings
    # ============================================================================

    @mcp.tool()
    def set_widget_design_size_mode(
        ctx: Context,
        widget_name: str,
        design_size_mode: str,
        custom_width: int = 1920,
        custom_height: int = 1080
    ) -> Dict[str, Any]:
        """
        Set the design size mode for a Widget Blueprint.

        Controls how the widget preview appears in the designer and how the widget
        determines its size at runtime.

        Args:
            widget_name: Name of the target Widget Blueprint
            design_size_mode: Design size mode. Options:
                - "DesiredOnScreen": Widget uses its desired size based on content
                - "Custom": Use custom width/height values (set in the designer)
                - "FillScreen": Fill entire screen (respects safe zones)
                - "CustomOnScreen": Custom size that responds to DPI scaling
            custom_width: Width when using Custom or CustomOnScreen modes (default: 1920)
            custom_height: Height when using Custom or CustomOnScreen modes (default: 1080)

        Returns:
            Dict containing:
                - success: Whether the operation succeeded
                - widget_name: Name of the modified widget
                - design_size_mode: The applied design size mode
                - width: Applied width value
                - height: Applied height value
                - message: Success or error description

        Examples:
            # Set a widget to fill the screen
            set_widget_design_size_mode(
                widget_name="WBP_MainHUD",
                design_size_mode="FillScreen"
            )

            # Set a widget to a specific custom size
            set_widget_design_size_mode(
                widget_name="WBP_InventoryPanel",
                design_size_mode="Custom",
                custom_width=800,
                custom_height=600
            )

            # Set a widget to use its content-based desired size
            set_widget_design_size_mode(
                widget_name="WBP_Tooltip",
                design_size_mode="DesiredOnScreen"
            )

            # Set a widget with DPI-aware custom size
            set_widget_design_size_mode(
                widget_name="WBP_MobileMenu",
                design_size_mode="CustomOnScreen",
                custom_width=1280,
                custom_height=720
            )
        """
        return set_widget_design_size_mode_impl(ctx, widget_name, design_size_mode, custom_width, custom_height)

    @mcp.tool()
    def set_widget_parent_class(
        ctx: Context,
        widget_name: str,
        new_parent_class: str
    ) -> Dict[str, Any]:
        """
        Change the parent class of a Widget Blueprint.

        Allows reparenting a widget to a different base class, which is useful for:
        - Switching from UserWidget to a custom base widget class
        - Inheriting from project-specific base widgets with shared functionality
        - Changing widget inheritance hierarchy

        Args:
            widget_name: Name of the target Widget Blueprint
            new_parent_class: New parent class to reparent to. Can be:
                - Simple class name: "UserWidget", "BaseUserWidget"
                - Full path: "/Script/UMG.UserWidget"
                - Blueprint path: "/Game/UI/Base/WBP_BaseWidget"

        Returns:
            Dict containing:
                - success: Whether the operation succeeded
                - widget_name: Name of the modified widget
                - new_parent_class: The new parent class
                - old_parent_class: The previous parent class
                - message: Success or error description

        Examples:
            # Reparent to a custom base widget
            set_widget_parent_class(
                widget_name="WBP_InventorySlot",
                new_parent_class="UInventorySlotEntryWidget"
            )

            # Reparent to a Blueprint-based base widget
            set_widget_parent_class(
                widget_name="WBP_GameMenu",
                new_parent_class="/Game/UI/Base/WBP_BaseMenuWidget"
            )

            # Reparent back to standard UserWidget
            set_widget_parent_class(
                widget_name="WBP_SimpleWidget",
                new_parent_class="UserWidget"
            )
        """
        return set_widget_parent_class_impl(ctx, widget_name, new_parent_class)

    # =========================================================================
    # Widget Animation Tools
    # =========================================================================

    @mcp.tool()
    def create_widget_animation(
        ctx: Context,
        widget_name: str,
        animation_name: str,
        duration: float = 1.0
    ) -> Dict[str, Any]:
        """
        Create a new animation in a Widget Blueprint.

        Widget animations use Unreal's Sequencer system (UWidgetAnimation) to animate
        widget properties over time. Animations can control opacity, transforms, colors,
        and other widget properties.

        Args:
            widget_name: Name of the target Widget Blueprint
            animation_name: Name for the new animation (e.g., "FadeIn", "SlideOut")
            duration: Duration of the animation in seconds (default: 1.0)

        Returns:
            Dict containing:
                - success: Whether the animation was created
                - widget_name: Name of the widget blueprint
                - animation_name: Name of the created animation
                - duration: Animation duration in seconds
                - message: Success or error description

        Examples:
            # Create a simple fade animation
            create_widget_animation(
                widget_name="WBP_MainMenu",
                animation_name="FadeIn",
                duration=0.5
            )

            # Create a longer slide animation
            create_widget_animation(
                widget_name="WBP_InventoryPanel",
                animation_name="SlideOut",
                duration=0.3
            )
        """
        return create_widget_animation_impl(ctx, widget_name, animation_name, duration)

    @mcp.tool()
    def add_animation_track(
        ctx: Context,
        widget_name: str,
        animation_name: str,
        target_component: str,
        property_name: str,
        track_type: str = "Float"
    ) -> Dict[str, Any]:
        """
        Add a property track to a widget animation.

        Tracks define which widget component property will be animated.
        Each track can have multiple keyframes that define values at specific times.

        Args:
            widget_name: Name of the target Widget Blueprint
            animation_name: Name of the animation to add the track to
            target_component: Name of the widget component to animate (e.g., "ContentBorder")
            property_name: Name of the property to animate. Common properties:
                - "RenderOpacity": Float 0-1 for fade effects
                - "RenderTransform": Transform for position/scale/rotation
                - "ColorAndOpacity": Color for tinting
            track_type: Type of track (default: "Float"):
                - "Float": Single float value (opacity, etc.)
                - "Vector2D": 2D vector (position, scale)
                - "Color": RGBA color value
                - "Transform": Full 2D transform (translation, scale, angle)

        Returns:
            Dict containing:
                - success: Whether the track was added
                - animation_name: Name of the animation
                - target_component: Component being animated
                - property_name: Property being animated
                - track_type: Type of track created
                - message: Success or error description

        Examples:
            # Add opacity track for fade effect
            add_animation_track(
                widget_name="WBP_MainMenu",
                animation_name="FadeIn",
                target_component="ContentBorder",
                property_name="RenderOpacity",
                track_type="Float"
            )

            # Add transform track for slide effect
            add_animation_track(
                widget_name="WBP_Tooltip",
                animation_name="SlideIn",
                target_component="TooltipPanel",
                property_name="RenderTransform",
                track_type="Transform"
            )
        """
        return add_animation_track_impl(ctx, widget_name, animation_name, target_component, property_name, track_type)

    @mcp.tool()
    def add_animation_keyframe(
        ctx: Context,
        widget_name: str,
        animation_name: str,
        target_component: str,
        property_name: str,
        time: float,
        value: Any
    ) -> Dict[str, Any]:
        """
        Add a keyframe to an animation track.

        Keyframes define the value of a property at a specific time in the animation.
        The animation interpolates between keyframes to create smooth transitions.

        Args:
            widget_name: Name of the target Widget Blueprint
            animation_name: Name of the animation
            target_component: Name of the widget component
            property_name: Name of the property (must match existing track)
            time: Time in seconds for the keyframe (0.0 = start)
            value: Value at this keyframe. Format depends on track type:
                - Float: Single number (e.g., 0.0, 1.0)
                - Vector2D: [x, y] array (e.g., [0.0, 100.0])
                - Color: [r, g, b, a] array, 0-1 range (e.g., [1.0, 1.0, 1.0, 0.0])
                - Transform: {"translation": [x, y], "scale": [x, y], "angle": degrees}

        Returns:
            Dict containing:
                - success: Whether the keyframe was added
                - animation_name: Name of the animation
                - time: Time of the keyframe
                - message: Success or error description

        Examples:
            # Fade from invisible to visible
            add_animation_keyframe(
                widget_name="WBP_MainMenu",
                animation_name="FadeIn",
                target_component="ContentBorder",
                property_name="RenderOpacity",
                time=0.0,
                value=0.0
            )
            add_animation_keyframe(
                widget_name="WBP_MainMenu",
                animation_name="FadeIn",
                target_component="ContentBorder",
                property_name="RenderOpacity",
                time=0.5,
                value=1.0
            )

            # Slide in from left
            add_animation_keyframe(
                widget_name="WBP_Panel",
                animation_name="SlideIn",
                target_component="Panel",
                property_name="RenderTransform",
                time=0.0,
                value={"translation": [-200.0, 0.0], "scale": [1.0, 1.0], "angle": 0.0}
            )
        """
        return add_animation_keyframe_impl(ctx, widget_name, animation_name, target_component, property_name, time, value)

    @mcp.tool()
    def set_animation_properties(
        ctx: Context,
        widget_name: str,
        animation_name: str,
        duration: float = -1.0,
        loop_mode: str = "",
        loop_count: int = 0
    ) -> Dict[str, Any]:
        """
        Set properties on a widget animation (duration, loop mode, etc.).

        Use this to modify animation playback behavior after creation.
        Only specified properties (non-default values) will be modified.

        Args:
            widget_name: Name of the target Widget Blueprint
            animation_name: Name of the animation to modify
            duration: New duration in seconds. Use -1 to skip (default: -1)
            loop_mode: Loop behavior. Options:
                - "Once": Play once and stop (default)
                - "Loop": Loop indefinitely or loop_count times
                - "PingPong": Play forward then backward
                - "": Skip (don't modify)
            loop_count: Number of loops when loop_mode is "Loop":
                - -1: Loop infinitely
                - 0: Skip (don't modify)
                - N > 0: Loop N times

        Returns:
            Dict containing:
                - success: Whether properties were modified
                - animation_name: Name of the animation
                - modified_properties: List of properties that were changed
                - message: Success or error description

        Examples:
            # Make animation loop infinitely
            set_animation_properties(
                widget_name="WBP_LoadingScreen",
                animation_name="SpinnerRotate",
                loop_mode="Loop",
                loop_count=-1
            )

            # Change duration and set ping-pong mode
            set_animation_properties(
                widget_name="WBP_Button",
                animation_name="Pulse",
                duration=0.8,
                loop_mode="PingPong"
            )
        """
        return set_animation_properties_impl(ctx, widget_name, animation_name, duration, loop_mode, loop_count)

    @mcp.tool()
    def get_widget_animations(
        ctx: Context,
        widget_name: str
    ) -> Dict[str, Any]:
        """
        Get all animations in a Widget Blueprint.

        Returns detailed information about all animations including their tracks
        and keyframes. Useful for inspecting existing animations or debugging.

        Args:
            widget_name: Name of the target Widget Blueprint

        Returns:
            Dict containing:
                - success: Whether retrieval was successful
                - widget_name: Name of the widget blueprint
                - animation_count: Number of animations found
                - animations: Array of animation info objects, each with:
                    - name: Animation name
                    - duration: Duration in seconds
                    - track_count: Number of tracks
                    - tracks: Array of track info with:
                        - target_component: Widget being animated
                        - property_name: Property being animated
                        - track_type: Type of track (Float, Color, etc.)
                        - keyframe_count: Number of keyframes
                - message: Success or error description

        Examples:
            # Get all animations from a menu widget
            get_widget_animations(widget_name="WBP_MainMenu")

            # Inspect animations in an inventory panel
            result = get_widget_animations(widget_name="WBP_InventoryPanel")
            if result["success"]:
                for anim in result["animations"]:
                    print(f"Animation: {anim['name']}, Duration: {anim['duration']}s")
        """
        return get_widget_animations_impl(ctx, widget_name)

    @mcp.tool()
    def delete_widget_animation(
        ctx: Context,
        widget_name: str,
        animation_name: str
    ) -> Dict[str, Any]:
        """
        Delete an animation from a Widget Blueprint.

        Permanently removes the animation and all its tracks and keyframes.
        This action cannot be undone via MCP (use Unreal's undo system if needed).

        Args:
            widget_name: Name of the target Widget Blueprint
            animation_name: Name of the animation to delete

        Returns:
            Dict containing:
                - success: Whether the animation was deleted
                - widget_name: Name of the widget blueprint
                - animation_name: Name of the deleted animation
                - message: Success or error description

        Examples:
            # Delete an old animation
            delete_widget_animation(
                widget_name="WBP_MainMenu",
                animation_name="OldFadeAnimation"
            )

            # Clean up test animations
            delete_widget_animation(
                widget_name="WBP_TestWidget",
                animation_name="TestAnimation"
            )
        """
        return delete_widget_animation_impl(ctx, widget_name, animation_name)

    logger.info("UMG tools registered successfully")

# Moved outside the function
logger.info("UMG tools module loaded")