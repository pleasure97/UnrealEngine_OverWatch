# MIT License
#
# Copyright (c) 2025 runeape.sats

from unreal_connection import get_unreal_connection
from unreal_utils import format_transform_parameters, get_common_actor_name, parse_keyword_arguments, \
                         validate_required_parameters, BASIC_SHAPES
import logging
from typing import Dict, Any, Optional
import json

# Configure Logging
logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger("UnrealActors")

def spawn_unreal_actor(actor_class: str, params: Dict[str, Any]) -> Optional[str]:
    """
    Base Function to Spawn an Actor from Any Class
    :param actor_class: Path to the Actor Class
    :param params: Dictionary of Parameters
    :return: Actor Path if Successful, None Otherwise
    """
    try:
        unreal = get_unreal_connection()

        # Format Transform Parameters
        transform = format_transform_parameters(params)

        # Actor Name
        name = get_common_actor_name(params)

        # Create Spawn Parameters
        spawn_parameters = {"ActorClass": actor_class}

        # Add Location if Provided
        if 'location' in transform:
            spawn_parameters["Location"] = transform['location']

        # Add Rotation if Provided
        if 'rotation' in transform:
            spawn_parameters["Rotation"] = transform['rotation']

        # Spawn the Actor
        spawn_result = unreal.send_command(
            "/Script/EditorScriptingUtilities.Default__EditorLevelLibrary",
            "SpawnActorFromClass",
            spawn_parameters)

        actor_path = spawn_result.get("ReturnValue", "")
        if not actor_path:
            logger.error(f"Failed to Spawn Actor of Class {actor_class}")
            return None

        # Set Actor Name
        unreal.send_command(actor_path, "SetActorLabel", {"NewActorLabel": name})

        # Set Scale if Provided
        # TODO - Why Not Include Scale in Spawn Parameter
        if 'scale' in transform:
            unreal.send_command(actor_path, "SetActorScale3D", {"NewScale3D": transform['scale']})
        return actor_path
    except Exception as e:
        logger.error(f"Error in spawn_unreal_actor(): {str(e)}")
        return None

def create_unreal_static_mesh_actor(keyword_arguments: Optional[str, Dict]) -> str:
    """
    Create New Static Mesh Actor with Basic Shape or Custom Mesh
    :param kwargs: String or Dict with Parameters
    :return: Success or Error Message
    """
    try:
        unreal = get_unreal_connection()
        params = parse_keyword_arguments(keyword_arguments)

        # Determine Mesh Type and Path
        mesh_type = params.get('mesh_type', 'CUBE').upper()
        mesh_path = params.get('static_mesh_asset_path') or params.get('static_mesh')

        # Use Basic Shape if there is no Explicit Mesh Path
        if not mesh_path:
            if mesh_type in BASIC_SHAPES:
                mesh_path = BASIC_SHAPES[mesh_type]
            else:
                return f"Error for Unsupported Mesh Type '{mesh_type}'. " \
                       f"Supported Types are {', '.join(BASIC_SHAPES.keys())}"

        # Get Actor Name
        name = get_common_actor_name(params, f"My {mesh_type.capitalize()}")

        # Spawn Actor
        actor_path = spawn_unreal_actor("/Script/Engine.StaticMeshActor", params)
        if not actor_path:
            return "Error: Failed to Spawn Static Mesh Actor"

        # Get Static Mesh Component
        component_path = unreal.get_component_by_class(actor_path, "/Script/Engine.StaticMeshComponent")
        if not component_path:
            return "Error: Failed to Get StaticMeshComponent"

        # Set Mesh
        unreal.send_command(component_path, "SetStaticMesh", {"NewMesh": mesh_path})

        # Set Material and Color if Provided
        material_override = params.get('material_override')
        color = params.get('color') or params.get('material_color')

        if material_override:
            unreal.send_command(component_path, "SetMaterial", {"ElementIndex": 0, "Material": material_override})
        elif color and isinstance(color, list) and len(color) >= 3:
            # Create Dynamic Material Instance
            create_material_result = unreal.send_command(
                component_path,
                "CreateDynamicMaterialInstance",
                {"ElementIndex": 0, "SourceMaterial": "/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"})
            material_path = create_material_result.get("ReturnValue", "")
            if material_path:
                # Set Color Parameter - with Proper RGBA Format
                color_parameter = {"R": color[0], "G": color[1], "B": color[2], "A": 1.}
                if len(color) >= 4:
                    color_parameter["A"] = color[3]
                unreal.send_command(
                    material_path,
                    "SetVectorParameterValue",
                    {"ParameterName": "Color", "Value": color_parameter})
            return f"Successfully Created {name} Actor at Position {params.get('location', [0, 0, 0])}"
    except Exception as e:
        logger.error(f"Error in create_unreal_static_mesh_actor(): {str(e)}")
        return f"Error Creating Static Mesh Actor : {str(e)}"

def spawn_unreal_static_mesh_actor_from_mesh(keyword_arguments: Optional[str, Dict]) -> str:
    """
    Spawn Static Mesh Actor using Existing Static Mesh Asset
    :param keyword_arguments: String or Dict with Parameters
    :return: Success or Error Message
    """
    try:
        params = parse_keyword_arguments(keyword_arguments)

        # Get Static Mesh Path
        static_mesh = params.get('static_mesh') or params.get('mesh')

        # Validate Required Parameters
        is_valid, error_message = validate_required_parameters(params, ['static_mesh'])
        if not is_valid:
            return error_message

        # Add Static Mesh Path to Parameters
        params['static_mesh_asset_path'] = static_mesh

        # Use Common Static Mesh Creation Function
        return create_unreal_static_mesh_actor(params)
    except Exception as e:
        logger.error(f"Error in spawn_unreal_static_mesh_actor_from_mesh(): {str(e)}")
        return f"Error Spawning Static Mesh Actor : {str(e)}"

def spawn_unreal_actor_from_blueprint(keyword_arguments: Optional[str, Dict]) -> str:
    """
    Spawn Actor from Blueprint Class
    :param keyword_arguments: String or Dict with Parameters
    :return: Success or Error Message
    """
    try:
        params = parse_keyword_arguments(keyword_arguments)

        # Get Actor Class
        actor_class = params.get('actor_class') or params.get('class')

        # Validate Required Parameters
        is_valid, error_message = validate_required_parameters(params, ['actor_class'])
        if not is_valid:
            return error_message

        # Spawn Actor
        actor_path = spawn_unreal_actor(actor_class, params)
        if not actor_path:
            return f"Error: Failed to Spawn Actor from Blueprint Class : {actor_class}"

        # Get Name
        name = get_common_actor_name(params, "BlueprintActor")

        return f"Successfully Created Actor '{name}' from Blueprint Class '{actor_class}

    except Exception as e:
        logger.error(f"Error in spawn_unreal_actor_from_blueprint() : {str(e)}")
        return f"Error Spawning Actor from Blueprint : {str(e)}"

def modify_unreal_actor(keyword_arguments: Optional[str, Dict]) -> str:
    """
    Modify Existing Actor in Level
    :param keyword_arguments: String or Dict with Parameters
    :return: Success or Error Message
    """
    try:
        unreal = get_unreal_connection()
        params = parse_keyword_arguments(keyword_arguments)

        # Get Actor Label
        actor_label = params.get('actor_label')

        # Validate Required Parameters
        is_valid, error_message = validate_required_parameters(params, ['actor_label'])
        if not is_valid:
            return error_message

        # Find Actor
        actor_path = unreal.find_actor_by_label(actor_label)
        if not actor_path:
            return f"Actor '{actor_label}' Not Found in Current Level"

        # Get Transform Parameters
        transform = format_transform_parameters(params)

        # Apply Location if Provided
        if 'location' in transform:
            unreal.send_command(actor_path, "SetActorLocation", {"NewLocation": transform['location']})

        # Apply Rotation if Provided
        if 'rotation' in transform:
            unreal.send_command(actor_path, "SetActorRotation", {"NewRotation": transform['rotation']})

        # Apply Scale if Provided
        if 'scale' in transform:
            unreal.send_command(actor_path, "SetActorScale3D", {"NewScale3D": transform['scale']})

        # Set Visibility if Provided
        visible = params.get('visible')
        if visible is not None:
            unreal.send_command(actor_path, "SetActorHiddenInGame", {"NewHidden": not visible})

        # Set Material Color if Provided
        color = params.get('color') or params.get('material_color')
        if color and isinstance(color, list) and len(color) >= 3:
            # Get Static Mesh Component if it Exists
            component_path = unreal.get_component_by_class(actor_path, "/Script/Engine.StaticMeshComponent")
            if component_path:
                # Create Dynamic Material Instance
                create_material_result = unreal.send_command(
                    component_path,
                    "CreateDynamicMaterialInstance",
                    {"ElementIndex": 0, "SourceMaterial": "/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"})
                material_path = create_material_result.get("ReturnValue", "")
                if material_path:
                    # Set Color Parameter with Proper RGBA Format
                    color_parameter = {"R": color[0], "G": color[1], "B": color[2], "A": 1.}
                    if len(color) >= 4:
                        color_parameter["A"] = color[3]
                    unreal.send_command(
                        material_path,
                        "SetVectorParameterValue",
                        {"ParameterName": "Color", "Value": color_parameter})
        return f"Succesfully Modified Actor : {actor_label}"
    except Exception as e:
        logger.error(f"Error in modify_unreal_actor() : {actor_label}")
        return f"Error modifying actor : {str(e)}"

def get_unreal_actor_info(actor_label: str) -> str:
    """
    Get Detailed Information about Actor
    :param actor_label: Label of Actor
    :return: JSON String with Actor Information
    """
    try:
        unreal = get_unreal_connection()
        # Find Actor
        actor_path = unreal.find_actor_by_label(actor_label)
        if not actor_path:
            return f"Actor '{actor_label}' Not Found in Current Level"

        # Get Basic Information
        information = {"path": actor_path, "label": actor_label}

        # Get Location
        try:
            location_result = unreal.send_command(actor_path, "GetActorLocation")
            information["location"] = location_result.get("ReturnValue", {})
        except Exception as e:
            logger.warning(f"Could Not Get Location from Actor {actor_path}: {str(e)}")
            information["location"] = "Not available"

        # Get Rotation
        try:
            rotation_result = unreal.send_command(actor_path, "GetActorRotation")
            information["rotation"] = rotation_result.get("ReturnValue", {})
        except Exception as e:
            logger.warning(f"Could Not Get Rotation from Actor {actor_path}: {str(e)}"
            information["rotation"] = "Not available"

        # Get Scale
        try:
            scale_result = unreal.send_command(actor_path, "GetActorScale3D")
            information["scale"] = unreal.send_command(actor_path, "GetActorScale3D")
        except Exception as e:
            logger.warning(f"Could Not Get Scale from Actor {actor_path} : {str(e)}")
            information["scale"] = "Not available"

        # Get Bounding Box
        try:
            # GetActorBounds() returns Origin and BoxExtent
            bounds_result = unreal.send_command(actor_path, "GetActorBounds", {"bOnlyCollidingComponents": False})
            if bounds_result:
                origin = bounds_result.get("Origin", {})
                box_extent = bounds_result.get("BoxExtent", {})
                # Calculate Minimum and Maximum Points of Bounding Box
                minimum_point = {
                    "X": origin.get("X", 0) - box_extent.get("X", 0),
                    "Y": origin.get("Y", 0) - box_extent.get("Y", 0),
                    "Z": origin.get("Z", 0) - box_extent.get("Z", 0)
                }

                maximum_point = {
                    "X": origin.get("X", 0) + box_extent.get("X", 0),
                    "Y": origin.get("Y", 0) + box_extent.get("Y", 0),
                    "Z": origin.get("Z", 0) + box_extent.get("Z", 0)
                }

                information["bounding_box"] = {
                    "origin": origin,
                    "extent": box_extent,
                    "min": minimum_point,
                    "max": maximum_point,
                    "size": {
                        "X": box_extent.get("X", 0) * 2,
                        "Y": box_extent.get("Y", 0) * 2,
                        "Z": box_extent.get("Z", 0) * 2
                    }
                }
            else:
                information["bounding_box"] = "Not available"
        except Exception as e:
            logger.warning(f"Could Not Get Bounding Box from Actor {actor_path} : {str(e)}")
            information["bounding_box"] = "Not available"

        # Determine Actor Type from Path
        actor_path = "Unknown"
        if "StaticMeshActor" in actor_path:
            actor_type = "StaticMeshACtor"
            # If it's a Static Mesh Actor, Get Mesh and Material Information
            component_path = unreal.get_component_by_class(actor_path, "/Script/Engine.StaticMeshComponent")
            if component_path:
                # Get Static Mesh Path
                try:
                    mesh_result = unreal.send_command(component_path, "GetStaticMesh")
                    information["static_mesh"] = mesh_result.get("ReturnValue", "")
                except Exception:
                    information["static_mesh"] = "Not available"
                # Get Material
                try:
                    material_result = unreal.send_command(component_path, "GetMaterial", {"ElementIndex": 0})
                    information["material"] = material_result.get("ReturnValue", "")
                except Exception:
                    information["material"] = "Not available"
                # Get Component Bounds for More Accurate Mesh Bounds
                try:
                    component_bounds_result = unreal.send_command(component_path, "GetBounds")
                    if component_bounds_result:
                        bounds = component_bounds_result.get("ReturnValue", {})
                        information["component_bounds"] = bounds
                except Exception as e:
                    logger.warning(f"Could Not Get Component Bounds for {component_path} : {str(e)}")
        elif "Light" in actor_path:
            actor_type = "Light"
        elif "PlayerStart" in actor_path:
            actor_type = "PlayerStart"
        elif "SkyAtmosphere" in actor_path:
            actor_type = "SkyAtmosphere"
        elif "SkyLight" in actor_path:
            actor_type = "SkyLight"
        elif "Fog" in actor_path:
            actor_type = "Fog"
        elif "VolumetricCloud" in actor_path:
            actor_type = "VolumetricCloud"

        information["actor_type"] = actor_type

        return json.dumps(information, indent=2)
    except Exception as e:
        logger.error(f"Error in get_unreal_actor_info() : {str(e)}")
        return f"Error Getting Actor Info : {str(e)}"

def delete_unreal_actor(actor_label: str) -> str:
    """
    Delete an Actor from Current Level by Its Label
    :param actor_label: Label of Actor to Delete
    :return: Success or Error Message
    """
    try:
        unreal = get_unreal_connection()
        # Find the Actor
        actor_path = unreal.find_actor_by_label(actor_label)
        if not actor_path:
            return f"Actor '{actor_label}' Not Found in the Current Level"
        result = unreal.send_command(actor_path, "DestroyActor", {"ActorTarget": actor_path})
    except Exception as e:
        logger.error(f"Error in Deleting Actor : {str(e)}")
        return f"Error Deleting Actor : {str(e)}"