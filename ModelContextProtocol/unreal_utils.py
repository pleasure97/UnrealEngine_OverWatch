# MIT License
#
# Copyright (c) 2025 runeape.sats
import logging
from typing import Dict, Any, List, Optional, Tuple
import json

# Configure Logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger("UnrealUtils")

# Configure BASIC SHAPES Dictionary
BASIC_SHAPES = {
    "CUBE": "/Engine/BasicShapes/Cube.Cube",
    "SPHERE": "/Engine/BasicShapes/Sphere.Sphere",
    "CYLINDER": "/Engine/BasicShapes/Cylinder.Cylinder",
    "PLANE": "/Engine/BasicShapes/Plane.Plane",
    "CONE": "/Engine/BasicShapes/Cone.Cone"
}

# Configure Asset Type Identifiers for Searching
ASSET_TYPE_IDENTIFIERS = {
    'blueprint': ['/blueprint', '/blueprints', 'bp_', '_bp'],
    'staticmesh': ['/mesh', '/meshes', '/staticmesh', '/staticmeshes', 'sm_', '_sm'],
    'material': ['/material', '/materials', 'mat_', '_mat', 'mt_', '_mt', 'm_'],
    'texture': ['/texture', '/textures', 't_', '_t'],
    'sound': ['/sound', '/sounds', '/audio', 's_', '_s'],
    'particle': ['/fx', '/effect', '/effects', '/particle', '/particles', 'fx_', 'p_', '_p'],
    'animation': ['/anim', '/animation', '/animations', 'a_', '_a']
}

# Common Subdirectories in Unreal Engine Projects for Asset Searches
COMMON_SUBDIRECTORIES = [
    "", # Basic Directory itself,
    "/Blueprints",
    "/Meshes",
    "/StaticMeshes",
    "/Materials",
    "/Textures",
    "/FX",
    "/Audio",
    "/Animations"
]

def format_transform_parameters(params: Dict[str, Any]) -> Dict[str, Dict[str, float]]:
    """
    Format Location, Rotation, and Scale Parameters for Unreal Engine.
    :param params: Dictionary of Parameters
    :return: Dictionary with Formatted Location, Rotation, and Scale
    """
    result = {}
    # Format Location
    location = params.get('location')
    if location:
        result['location'] = vector_to_ue_format(location)
    # Format Rotation
    rotation = params.get('rotation')
    if rotation:
        result['rotation'] = vector_to_ue_format(rotation, ["Pitch", "Yaw", "Roll"])
    # Format Scale
    scale = params.get('scale')
    if scale:
        result['scale'] = vector_to_ue_format(scale)

    return result


def vector_to_ue_format(vector: List[float], keys: List[str] = None) -> Dict[str, float]:
    """
    Convert a Vector List [x, y, z] to Unreal Engine Format {"X": x, "Y": y, "Z": z}
    :param vector: List of Float Values
    :param keys: Optional List of Custom Keys (default : ["X", "Y", "Z"])
    :return: Dictionary in Unreal Engine Format
    """
    if not keys:
        keys = ["X", "Y", "Z"]
    if not isinstance(vector, list) or len(vector) < len(keys):
        # Return Default Values if Vector is Invalid
        return {key: 0. if key != "A" else 1. for key in keys}
    # Ensure All Values are floats
    result = {}
    for idx, key in enumerate(keys):
        if idx < len(vector):
            result[key] = float(vector[idx])
        elif key == "A":
            result[key] = 1.
        else:
            result[key] = 0.
    return result


def get_common_actor_name(params: Dict[str, Any], default_name: str = "NewActor") -> str:
    """
    Get Actor Name from Parameters, Checking Common Variations
    :param params: Dictionary of Parameters
    :param default_name: Default Name if None is Specified
    :return: Actor Name to Use
    """
    return params.get('actor_label') or params.get('name') or params.get('label') or default_name


def parse_keyword_arguments(keyword_arguments: Optional[str, Dict]) -> Dict[str, Any]:
    """
    Parse Keyword Arguments from string, dict, or JSON format to Unified Dictionary
    :param keyword_arguments: String with key=value pairs, JSON string, or dictionary
    :return: Dictionary of Parsed Parameters
    """
    if not keyword_arguments:
        return {}

    # Check if keyword_arguments is already dictionary type
    if isinstance(keyword_arguments, dict):
        return keyword_arguments

    # Check if keyword_arguments is a JSON string
    if isinstance(keyword_arguments, str):
        if keyword_arguments.strip().startswith('{') and keyword_arguments.strip().endswith('}'):
            try:
                return json.loads(keyword_arguments)
            except json.JSONDecodeError:
                logger.warning(f"Failed to parse as JSON : {keyword_arguments}")

    # Parse as Space-Separated key=value pairs
    kwargs = {}

    if isinstance(keyword_arguments, str):
        parts = kwargs.split()
        for part in parts:
            if '=' in part:
                key, value = part.split('=', 1)
                kwargs[key] = parse_value(key, value)

    return kwargs


def parse_value(key: str, value: str) -> Any:
    """
    Parse a String Valu e into the Appropriate Type Based on Key and Content
    :param key: Parameter Key Name
    :param value: String Value to Parse
    :return: Parsed Value in Appropriate Type 
    """
    # Parse Vectors (Location, Rotation, Scale, and Color)
    if ',' in value and key in ['location', 'rotation', 'scale', 'color', 'material_color']:
        return [float(val) for val in value.split(',')]

    # Parse Booleans
    if value.lower() == 'true':
        return True
    if value.lower() == 'false':
        return False

    # Parse Numbers
    if value.isdigit():
        return int(value)
    # TODO - Use regex
    if value.replace('.', '', 1).isdigit():
        return float(value)

    return value

def validate_required_parameters(params: Dict[str, Any], required_keys: List[str]) -> Tuple[bool, str]:
    """
    Validate that Required Parameters are Present
    :param params: Dictionary of Parameters
    :param required_keys: List of Required Parameter Keys
    :return: Tuple of (is_valid, error_message)
    """
    missing = [required_key for required_key in required_keys if not params.get(required_key)]
    if missing:
        return False, f"Missing Required Parameters : {', '.join(missing)}"
    return True, ""
