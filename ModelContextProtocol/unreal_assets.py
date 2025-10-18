# MIT License
#
# Copyright (c) 2025 runeape.sats
# Functions for Working with Unreal Engine Assets
import logging
from typing import Optional, Dict
from unreal_connection import get_unreal_connection
from unreal_utils import parse_keyword_arguments, ASSET_TYPE_IDENTIFIERS, COMMON_SUBDIRECTORIES
import json

# Configure Logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger("UnrealAssets")

def get_available_assets(keyword_arguments) -> str:
    """
    Get List of Available Assets of Specific Type in Project
    :param keyword_arguments: String or Dict with Parameters:
        - asset_type: Type of Assets to List
        - search_path: Optional Path to Search in
        - search_term: Optional Term to Filter by
        - max_results: Maximum Number of Results
        - recursive: Whether to Search Recursively
    :return: JSON String with Matching Assets
    """
    try:
        unreal = get_unreal_connection()
        params = parse_keyword_arguments(keyword_arguments)

        # Get Parameters
        asset_type = params.get('asset_type', 'All').lower()
        search_path = params.get('search_path', '/Game')
        search_term = params.get('search_term', '')
        max_results = params.get('max_results', 20)
        recursive = params.get('recursive', True)

        # Convert String Boolean to Actual Boolean if Needed
        if isinstance(recursive, str):
            recursive = recursive.lower() == 'true'

        # Use Editor Asset Library to Get Available Assets
        try:
            # Get Assets in Specified Path
            list_assets_result = unreal.send_command(
                "/Script/EditorScriptingUtilities.Default__EditorAssetLibrary",
                "ListAssets",
                {"DictionaryPath": search_path, "Recursive": recursive, "IncludeFolder": True})
            assets = list_assets_result.get("ReturnValue", [])
            logger.info(f"Found {len(assets)} Total Assets in {search_path}")

            # Filter Assets by Type and Search Term
            filtered_assets = []

            for asset_path in assets:
                if not asset_path:
                    continue
                # Path Lowercase for Case-Insensitive Matching
                asset_path_lower = asset_path.lower()

                # Check Asset Type if Specified
                asset_type_match = True
                if asset_type != 'all' and asset_type in ASSET_TYPE_IDENTIFIERS:
                    identifiers = ASSET_TYPE_IDENTIFIERS[asset_type]
                    # Check if Any of Asset Type Identifiers Exist in Path
                    if not any(identifier in asset_path_lower for identifier in identifiers):
                        asset_type_match = False

                # Check for Search Term Match if Specified
                search_term_match = True
                if search_term and search_term.lower() not in asset_path_lower:
                    search_term_match = False

                # Add Asset to Filtered List if it Matches All Criteria
                if asset_type_match and search_term_match:
                    filtered_assets.append(asset_path)

                # Stop if we've Reached Max Results
                if len(filtered_assets) >= max_results:
                    break

            # Prepare Response
            result = {
                "asset_type": asset_type.capitalize() if asset_type != 'all' else "ALL",
                "search_path": search_path,
                "search_term": search_term,
                "total_found": len(filtered_assets),
                "assets": filtered_assets
            }

            return json.dumps(result, indent=2)
        except Exception as e:
            logger.error(f"Error using Editor Asset Library : {str(e)}")
            # Fallback to Different Approach - Try Using GetAssetsByPath
            try:
                get_assets_result = unreal.send_command(
                    "/Script/EditorScriptingUtilities.Default__EditorAssetLibrary",
                    "GetAssetsByPath",
                    {
                        "DirectoryPath": search_path,
                        "Recursive": recursive,
                        "IncludeFolder": True
                    })
                assets = get_assets_result.get("ReturnValue", [])

                # Filter Assets as Before
                filtered_assets = []
                for asset_path in assets:
                    if not asset_path:
                        continue

                    asset_path_lower = asset_path.lower()

                    # Check asset type
                    asset_type_match = True
                    if asset_type != 'all' and asset_type in ASSET_TYPE_IDENTIFIERS:
                        identifiers = ASSET_TYPE_IDENTIFIERS[asset_type]
                        if not any(identifier in asset_path_lower for identifier in identifiers):
                            asset_type_match = False

                    # Check search term
                    search_term_match = True
                    if search_term and search_term.lower() not in asset_path_lower:
                        search_term_match = False

                    # Add to filtered list if matching
                    if asset_type_match and search_term_match:
                        filtered_assets.append(asset_path)

                    # Check max results
                    if len(filtered_assets) >= max_results:
                        break

                result = {
                    "asset_type": asset_type.capitalize() if asset_type != 'all' else "All",
                    "search_path": search_path,
                    "search_term": search_term,
                    "total_found": len(filtered_assets),
                    "assets": filtered_assets
                }

                return json.dumps(result, indent=2)

            except Exception as e2:
                logger.error(f"Alternative approach also failed: {str(e2)}")
                return f"Error listing assets: {str(e)}. Alternative approach also failed: {str(e2)}"

    except Exception as e:
        logger.error(f"Error getting available assets: {str(e)}")
        return f"Error getting available assets: {str(e)}"

def search_unreal_assets_recursively(
        base_path: str,
        asset_type: str = None,
        search_term: str = None,
        max_results: int = 50) -> str:
    """
    Search for Assets in All Common Subdirectories of Base Path
    :param base_path: Base Path to Search in
    :param asset_type: Optional Type of Assets to Filter by
    :param search_term: Optional Term to Filter Results
    :param max_results: Maximum Number of Results
    :return: JSON String with Matched Assets
    """
    # Prepare Keyword Arguments for Each Search
    if asset_type:
        asset_type_param = f"asset_type={asset_type} "
    else:
        asset_type_param = ""

    if search_term:
        search_term_param = f"search_term={search_term} "
    else:
        search_term_param = ""

    max_results_param = f"max_results={max_results}"

    # Combined Assets from All Subdirectories
    all_assets = []

    # Search in Each Subdirectory
    for subdirectory in COMMON_SUBDIRECTORIES:
        search_path = f"{base_path}{subdirectory}"
        keyword_arguments = f"{asset_type_param}search_path={search_path} {search_term_param}{max_results_param}"
        try:
            # Get Assets in this Subdirectory
            available_assets = get_available_assets(keyword_arguments)
            result = json.loads(available_assets)

            # Add Assets to Combined List
            if result and "assets" in result:
                found_assets = result.get("assets", [])
                all_assets.extend(found_assets)
                logger.info(f"Found {len(found_assets)} Assets in {search_path}")
        except Exception as e:
            logger.warning(f"Error Searching in {search_path}: {str(e)}")
            continue

    # Remove Duplicates while Preserving Order
    unique_assets = []
    for asset in all_assets:
        if asset not in unique_assets:
            unique_assets.append(asset)

    # Prepare Combined Result
    combined_result = {
        "asset_type": asset_type.capitalize() if asset_type else "All",
        "search_path": base_path,
        "search_term": search_term or "",
        "total_found": len(unique_assets),
        "assets": unique_assets[:max_results]
    }

    return json.dumps(combined_result, indent=2)

def get_unreal_level_information() -> str:
    """
    Get Information about Current Level
    :return: JSON String with Level Information
    """
    try:
        unreal = get_unreal_connection()

        # Get All Level Actors
        actors_result = unreal.send_command("/Script/UnrealEd.Default__EditorActorSubsystem", "GetAllLevelActors")
        actors = actors_result.get("ReturnValue", [])

        # Get Details for Each Actor
        actors_information = []

        for actor_path in actors:
            try:
                # Store Basic Information
                actor_information = {"path": actor_path}
                # Try to Get Actor Label
                try:
                    label_result = unreal.send_command(actor_path, "GetActorLabel")
                    actor_information["label"] = label_result.get("ReturnValue", "Unknown")
                except Exception as e:
                    logger.warning(f"Could Not Get Label for Actor {actor_path}: {str(e)}")
                    # Extract Name from Path as Fallback
                    try:
                        actor_information["label"] = actor_path.split('.')[-1]
                    except:
                        actor_information["label"] = "Unknown"

                # Try to Get Actor Location
                try:
                    location_result = unreal.send_command(actor_path, "GetActorLocation")
                    actor_information["location"] = location_result.get("ReturnValue", {})
                except Exception as e:
                    logger.warning(f"Could Not Get Location for Actor {actor_path} : {str(e)}")
                    actor_information["location"] = "Unknown"

                # Infer Type from Path
                actor_type = "Unknown"
                if "StaticMeshActor" in actor_path:
                    actor_type = "StaticMeshActor"
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

                actor_information["type"] = actor_type
                actors_information.append(actor_information)
            except Exception as e:
                logger.warning(f"Error Getting Details from Actor {actor_path}: {str(e)}")
                actors_information.append({"path": actor_path, "error": str(e)})

        # Get Current Level Information
        level_name = "Unknown"
        if actors and len(actors) > 0:
            try:
                # Extract Level Name from the First Actor's Path
                path_parts = actors[0].split(':')
                if len(path_parts) > 0:
                    map_part = path_parts[0]
                    level_name = map_part.split('.')[-1]
            except Exception as e:
                logger.warning(f"Error Extracting Level Name: {str(e)}")

        # Compile Level Information
        level_information = {
            "level_name": level_name,
            "actor_count": len(actors),
            "actors": actors_information
        }

        return json.dumps(level_information, indent=2)
    except Exception as e:
        logger.error(f"Error Getting Level Information: {str(e)}")
        return f"Error Getting Level Information: {str(e)}"

