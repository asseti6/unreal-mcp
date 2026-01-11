"""
Widget Animation Utilities for Unreal MCP.

This module provides utilities for creating and managing UMG Widget Animations in Unreal Engine.
"""

import logging
from typing import Dict, List, Any, Union
from mcp.server.fastmcp import Context
from utils.unreal_connection_utils import send_unreal_command

# Get logger
logger = logging.getLogger("UnrealMCP")


def create_widget_animation(
    ctx: Context,
    widget_name: str,
    animation_name: str,
    duration: float = 1.0
) -> Dict[str, Any]:
    """
    Create a new animation in a Widget Blueprint.

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint
        animation_name: Name for the new animation
        duration: Duration of the animation in seconds (default: 1.0)

    Returns:
        Dict containing success status and animation information
    """
    params = {
        "widget_name": widget_name,
        "animation_name": animation_name,
        "duration": duration
    }

    logger.info(f"Creating widget animation '{animation_name}' in widget '{widget_name}' (Duration: {duration}s)")
    return send_unreal_command("create_widget_animation", params)


def add_animation_track(
    ctx: Context,
    widget_name: str,
    animation_name: str,
    target_component: str,
    property_name: str,
    track_type: str = "Float"
) -> Dict[str, Any]:
    """
    Add a property track to an animation.

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint
        animation_name: Name of the animation to add the track to
        target_component: Name of the widget component to animate
        property_name: Name of the property to animate (e.g., "RenderOpacity")
        track_type: Type of track: "Float", "Vector2D", "Color", "Transform" (default: "Float")

    Returns:
        Dict containing success status and track information
    """
    params = {
        "widget_name": widget_name,
        "animation_name": animation_name,
        "target_component": target_component,
        "property_name": property_name,
        "track_type": track_type
    }

    logger.info(f"Adding {track_type} track for '{property_name}' on '{target_component}' to animation '{animation_name}'")
    return send_unreal_command("add_animation_track", params)


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

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint
        animation_name: Name of the animation
        target_component: Name of the widget component
        property_name: Name of the property to keyframe
        time: Time in seconds for the keyframe
        value: Value at this keyframe. Type depends on track:
               - Float track: single number (e.g., 1.0)
               - Vector2D track: [x, y] array
               - Color track: [r, g, b, a] array (0-1 range)
               - Transform track: {"translation": [x,y], "scale": [x,y], "angle": deg}

    Returns:
        Dict containing success status and keyframe information
    """
    params = {
        "widget_name": widget_name,
        "animation_name": animation_name,
        "target_component": target_component,
        "property_name": property_name,
        "time": time,
        "value": value
    }

    logger.info(f"Adding keyframe at {time}s for '{property_name}' on '{target_component}' in animation '{animation_name}'")
    return send_unreal_command("add_animation_keyframe", params)


def set_animation_properties(
    ctx: Context,
    widget_name: str,
    animation_name: str,
    duration: float = -1.0,
    loop_mode: str = "",
    loop_count: int = 0
) -> Dict[str, Any]:
    """
    Set properties on an animation (duration, loop mode, etc.).

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint
        animation_name: Name of the animation
        duration: New duration in seconds (-1 to skip)
        loop_mode: Loop mode: "Once", "Loop", "PingPong" (empty to skip)
        loop_count: Number of loops (-1 for infinite, 0 to skip)

    Returns:
        Dict containing success status and modified properties
    """
    params = {
        "widget_name": widget_name,
        "animation_name": animation_name,
        "duration": duration,
        "loop_mode": loop_mode,
        "loop_count": loop_count
    }

    logger.info(f"Setting properties on animation '{animation_name}' in widget '{widget_name}'")
    return send_unreal_command("set_animation_properties", params)


def get_widget_animations(
    ctx: Context,
    widget_name: str
) -> Dict[str, Any]:
    """
    Get all animations in a Widget Blueprint.

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint

    Returns:
        Dict containing success status and array of animation info objects:
        - name: Animation name
        - duration: Animation duration in seconds
        - track_count: Number of tracks in the animation
        - tracks: Array of track info (target component, property, type)
    """
    params = {
        "widget_name": widget_name
    }

    logger.info(f"Getting animations from widget '{widget_name}'")
    return send_unreal_command("get_widget_animations", params)


def delete_widget_animation(
    ctx: Context,
    widget_name: str,
    animation_name: str
) -> Dict[str, Any]:
    """
    Delete an animation from a Widget Blueprint.

    Args:
        ctx: MCP context
        widget_name: Name of the target Widget Blueprint
        animation_name: Name of the animation to delete

    Returns:
        Dict containing success status and deleted animation name
    """
    params = {
        "widget_name": widget_name,
        "animation_name": animation_name
    }

    logger.info(f"Deleting animation '{animation_name}' from widget '{widget_name}'")
    return send_unreal_command("delete_widget_animation", params)
