# MIT License
#
# Copyright (c) 2025 runeape.sats

import logging
from contextlib import asynccontextmanager
from mcp.server.fastmcp import FastMCP, Context
from typing import AsyncIterator, Dict, Any
from unreal_connection import get_unreal_connection
import json
import traceback

# Configure Logging 
logging.basicConfig(
    level=logging.INFO, 
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger("UnrealMCPServer")

# Global Spatial Context to Track All Actors 
spatial_context: Dict[str, Dict[str, str]] = {}

@asynccontextmanager
async def server_lifespan(server: FastMCP) -> AsyncIterator[Dict[str, Any]]:
    """ Manage Server Startup and Shutdown LifeCycle """
    global spatial_context
    try:
        logger.info("Unreal with Model Context Protocol Server Starting Up")
        try:
            unreal = get_unreal_connection()
            if unreal.test_connection():
                logger.info("Successfully Connected to Unreal Engine in server_lifespan()")
            else:
                logger.warning("Could not connect to Unreal Engine in server_lifespan()")
        # TODO - Specify Exception
        except Exception as e:
            logger.warning(f"Could not connect to Unreal Engine in server_lifespan() : {str(e)}")

        # Initialize Spatial Context
        spatial_context = {}
        yield {}
    finally:
        logger.info("Unreal with Model Context Protocol Shutdowns")
        spatial_context.clear()

mcp = FastMCP(
    "unreal_with_model_context_protocol",
    description="Unreal Engine Integration with Spatial Context",
    server_lifespan=server_lifespan
)

@mcp.tool()
def get_spatial_context(context: Context) -> str:
    """ Return Current Spatial Context of All Actors as a JSON string. """
    global spatial_context
    try:
        return json.dumps(spatial_context, indent=2)
    except Exception as e:
        logger.error(f"Error in get_spatial_context() : {str(e)}")
        return f"Error Retrieving Spatial Context : {str(e)}"

@mcp.tool()
def reset_spatial_context(context: Context) -> str:
    """ Reset Spatial Context, Clearing All Tracked Actors. """
    global spatial_context
    try:
        spatial_context.clear()
        return "Spatial Context Reset Successfully"
    except Exception as e:
        logger.error(f"Error in reset_spatial_context() : {str(e)}")
        return f"Error resetting spatial context : {str(e)}"

@mcp.tool()
def delete_actor(context: Context, actor_label: str) -> str:
    """
    Delete a Specific Actor from Unreal Engine Level.
    :param actor_label: The Label and Name of the Actor to Delete
    :return: Result which Contains Success or Failure Message
    """
    global spatial_context
    try:
        from unreal_actors import delete_unreal_actor
        result = delete_unreal_actor(actor_label)
        spatial_context.pop(actor_label, None)
        return result
    except Exception as e:
        logger.error(f"Error in delete_actor() : {str(e)}")
        return f"Error Deleting Actor : {str(e)}"

@mcp.tool()
def spawn_actor_from_blueprint(context: Context, keyword_arguments: str) -> str:
    """
    Spawn Level Actor Based on Unreal Blueprint Class
    :param context:
    :param keyword_arguments: String Containing Parameters as key=value Pairs or JSON Object
    Example : "actor_class=/Game/AssetName/Blueprints/BP_House0.BP_House0_C location=100,100,0 name=MyHouse"

    Supported Parameters:
    - actor_class: (required) Path to Blueprint Class
    - actor_label / name : Name for Actor
    - location : x, y, z Location Coordinates
    - rotation : Pitch, Yaw, Roll Rotation in Degrees
    - scale : x, y, z Scale Factors
    :return:
    """
    global spatial_context
    try:
        from unreal_actors import spawn_unreal_actor_from_blueprint
        result = spawn_unreal_actor_from_blueprint(keyword_arguments)
        # Parse Keyword Arguments to Update Spatial Context
        params = dict(keyword_argument.split("=") for keyword_argument in keyword_arguments.split() if "=" in keyword_argument)
        actor_label = params.get("actor_label", params.get("name", f"Actor_{len(spatial_context)}"))
        spatial_context[actor_label] = {
            "location": params.get("location", "0,0,0"),
            "rotation": params.get("rotation", "0,0,0"),
            "scale": params.get("scale", "1,1,1")
        }
        return result
    except Exception as e:
        logger.error(f"Error in spawn_actor_from_blueprint() : {str(e)}")
        return f"Error Spawning Actor from Blueprint : {str(e)}"

@mcp.tool()
def spawn_static_mesh(context: Context, keyword_arguments: str) -> str:
    """
    Spawn Static Mesh Actor using Existing Static Mesh Asset from Content Browser
    :param context:
    :param keyword_arguments: String Containing Parameters as key=value Pairs or JSON Object
        Example: "static_mesh=/Game/AssetName/Meshes/Bench01 location=100,100,0 name=MyBench"
    Supported Parameters:
    - static_mesh: (required) Path to Static Mesh Asset
    - actor_label / name: Name for Actor
    - location: x, y, z Location Coordinates
    - rotation: Pitch, Yaw, Roll Rotation in Degrees
    - scale: x, y, z Scale Factors
    - material_override: Path to Material to Use
    - color: RGB Color Values (0.-1.)
    :return:
    """
    global spatial_context
    try:
        from unreal_actors import spawn_unreal_static_mesh_actor_from_mesh
        result = spawn_unreal_static_mesh_actor_from_mesh(keyword_arguments)
        params = dict(keyword_argument.split("=") for keyword_argument in keyword_arguments.split() if "=" in keyword_argument)
        actor_label = params.get("actor_label", params.get("name", f"Mesh_{len(spatial_context)}"))
        spatial_context[actor_label] = {
            "location": params.get("location", "0,0,0"),
            "rotation": params.get("rotation", "0,0,0"),
            "scale": params.get("scale", "1,1,1")
        }
        return result
    except Exception as e:
        logger.error(f"Error in spawn_static_mesh() : {str(e)}")
        return f"Error Spawning Static Mesh Actor : {str(e)}"

@mcp.tool()
def create_static_mesh_actor(context:Context, keyword_arguments: str) -> str:
    """
    Create New Static Mesh Actor in Unreal Engine
    :param context:
    :param keyword_arguments: String Containing Parameters as key=value Pairs or JSON Object
        Example: "actor_label=Cube mesh_type=CUBE location=0,0,0"

    Supported Parameters:
    - actor_label / name: Name for Actor
    - mesh_type: One of CUBE, SPHERE, CYLINDER, PLANE, or CONE
    - location: x, y, z Location Coordinates
    - rotation: Pitch, Yaw, Roll Rotation in Degrees
    - scale: x, y, z Scale Factors. 1 means Same Scale
    - color: RGB Color Values (0.-1.)
    :return:
    """
    global spatial_context
    try:
        from unreal_actors import create_unreal_static_mesh_actor
        result = create_unreal_static_mesh_actor(keyword_arguments)
        params = dict(keyword_argument.split("=") for keyword_argument in keyword_arguments.split() if "=" in keyword_argument)
        actor_label = params.get("actor_label", params.get("name", f"Mesh_{len(spatial_context)}"))
        spatial_context[actor_label] = {
            "location": params.get("location", "0,0,0"),
            "rotation": params.get("rotation", "0,0,0"),
            "scale": params.get("scale", "1,1,1")
        }
        return result
    except Exception as e:
        logger.error(f"Error in create_static_mesh_actor(): {str(e)}")
        return f"Error Creating Static Mesh Actor: {str(e)}"

@mcp.tool()
def modify_actor(context: Context, keyword_arguments: str) -> str:
    """
    Modify Existing Actor in Unreal Engine Level
    :param context:
    :param keyword_arguments: String Containing Parameters as key=value Pairs or JSON Object
        Example: "actor_label=Cube location=100,200,50 rotation=0,45,0"

    Supported Parameters:
    - actor_label: (Required) Label / Name for Actor to Modify
    - location: x, y, z Location Coordinates
    - rotation: Pitch, Yaw, Roll Rotation in Degrees
    - scale: x, y, z Scale Factors
    - visible: true / false to Set Visibility
    - color: RGB Color Values (0.-1.)
    :return:
    """
    global spatial_context
    try:
        from unreal_actors import modify_unreal_actor
        result = modify_unreal_actor(keyword_arguments)
        params = dict(keyword_argument.split("=") for keyword_argument in keyword_arguments.split() if "=" in keyword_argument)
        actor_label = params["actor_label"]
        if actor_label in spatial_context:
            spatial_context[actor_label].update({
                param: params[param] for param in ["location", "rotation", "scale"] if param in params
            })
        return result
    except Exception as e:
        logger.error(f"Error in modify_actor() : {str(e)}")
        return f"Error Modifying Actor : {str(e)}"

@mcp.tool()
def get_actor_info(context: Context, actor_label: str) -> str:
    """
    Get Detailed Information about Specific Actor in Unreal Engine Level
    :param context:
    :param actor_label: Label / Name of Actor to Get Information about
    :return:
    """
    try:
        from unreal_actors import get_unreal_actor_info
        return get_unreal_actor_info(actor_label)
    except Exception as e:
        logger.error(f"Error in get_actor_info() : {str(e)}")
        return f"Error Getting Actor Info : {str(e)}"

@mcp.tool()
def get_level_information(context: Context) -> str:
    """
    Get Information about Current Unreal Engine Level and Update Spatial Context
    :param context:
    :return:
    """
    global spatial_context
    try:
        from unreal_assets import get_unreal_level_information
        # Get Level Information from Unreal Engine
        level_information = get_unreal_level_information()
        # Assume Level Information is JSON String or Similar Format with Actor Data
        # If it's Not JSON, you'd Need to Adjust Parsing Logic Accordingly
        try:
            # Parse Level Information if it's JSON
            level_data = json.loads(level_information)
            if isinstance(level_data, dict) and "actors" in level_data:
                # Clear Existing Spatial Context and Update with New Actor Data
                spatial_context.clear()
                for actor in level_data["actors"]:
                    actor_label = actor.get("actor_label", actor.get("name", f"Actor_{len(spatial_context)}"))
                    spatial_context[actor_label] = {
                        "location": actor.get("location", "0,0,0"),
                        "rotation": actor.get("rotation", "0,0,0"),
                        "scale": actor.get("scale", "1,1,1")
                    }
        except json.JSONDecodeError:
            # If Level Information isn't JSON or doesn't Contain Actor Data, Just Return it as-is
            logger.info("Level Information Not in Expected JSON Format, Spatial Context Unchanged")

        return level_information
    except Exception as e:
        logger.error(f"Error in get_level_information(): {str(e)}")
        return f"Error Getting Level Information: {str(e)}"

@mcp.tool()
def list_available_assets(context: Context, keyword_arguments: str) -> str:
    """
    List Available Assets of Specific Type in Unreal Engine Project
    :param context:
    :param keyword_arguments: String Containing Parameters as key=value Pairs or JSON Object
    Supported Parameters:
    - asset_type: Type of Assets to List (BlueprintClass, StaticMesh, Material, etc.)
    - search_path: Optional Path to Search for Assets (default : /Game)
    - search_term: Optional Term to Filter Results
    - max_results: Maximum Number of Results to Return (default: 20)
    :return:
    """
    try:
        from unreal_assets import get_available_assets
        return get_available_assets(keyword_arguments)
    except Exception as e:
        logger.error(f"Error in list_available_assets(): {str(e)}")
        return f"Error Listing Available Assets: {str(e)}"

@mcp.tool()
def search_assets_recursively(context: Context,
                              base_path: str, asset_type: str = None,
                              search_term: str = None, max_results: int = 50) -> str:
    """
    Search for Assets Recursively in All Common Subdirectories
    :param context:
    :param base_path: Base Path to Search in (e.g., '/Game/Blueprints')
    :param asset_type: Optional Type of Assets to Filter by
    :param search_term: Optional Search Term to Filter Results
    :param max_results: Maximum Number of Results (default : 50)
    :return:
    """
    try:
        from unreal_assets import search_unreal_assets_recursively
        return search_unreal_assets_recursively(base_path, asset_type, search_term, max_results)
    except Exception as e:
        logger.error(f"Error in search_assets_recursively(): {str(e)}")
        return f"Error Searching Assets Recursively : {str(e)}"

if __name__ == "__main__":
    try:
        logger.info("Starting Unreal Model Context Protocol Server...")
        mcp.run()
    except KeyboardInterrupt:
        logger.info("Server Stopped by User")
    except Exception as e:
        logger.error(f"Error Running Unreal Model Context Protocol Server: {str(e)}")
        traceback.print_exc()
