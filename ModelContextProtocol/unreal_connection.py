# MIT License
#
# Copyright (c) 2025 runeape.sats

import logging
import requests
from typing import Optional, Dict, Any, List

# Configure Logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger("UnrealConnection")

class UnrealConnection:
    """ Class to Manage Connection to Unreal Engine Remote Control API """
    def __init__(self,
                 host: str="127.0.0.1",
                 port: int=30010):
        self.host = host
        self.port = port
        self.base_url = f"http://{host}:{port}/remote/object/call"

    def test_connection(self) -> bool:
        # Test Connection to Unreal Engine Remote Control API
        try:
            # Get All Level Actors as Simple Test
            # TODO - EditorActorSubsystem -> Runtime?
            payload = {
                "objectPath": "/Script/UnrealEd.Default_EditorActorSubsystem",
                "functionName": "GetAllLevelActors"
            }
            response = requests.put(self.base_url, json=payload, timeout=10)
            response.raise_for_status()

            logger.info(f"Successfully Connected to Unreal Engine at {self.host}:{self.post}")
            return True
        # TODO - Specify Exception
        except Exception as e:
            logger.error(f"Failed to Connect to Unreal Engine : {str(e)}")
            return False

    def send_command(self,
                     object_path: str,
                     function_name: str,
                     parameters: Dict[str, Any],
                     generate_transaction: bool = True) -> Dict[str, Any]:
        """
        Send a Command to Unreal Engine and Return the Response
        :param object_path: Path to UE Object
        :param function_name: Name of Function to Call
        :param parameters: Dictionary of Parameters to Pass
        :param generate_transaction: Whether to Generate Transaction for Undo
        :return: Dictionary with the Response from Unreal Engine
        """
        payload = {
            "objectPath": object_path,
            "functionName": function_name,
            "parameters": parameters or {},
            "generateTransaction": generate_transaction
        }

        try:
            # Log Command
            if parameters:
                logger.info(f"Sending Command to Unreal Engine : {function_name} with params {parameters}")
            else:
                logger.info(f"Sending Command to Unreal Engine : {function_name}")
            # Send the Command
            response = requests.put(self.base_url, json=payload, timeout=10)
            response.raise_for_status()
            # Save the Response as JSON Object
            result = response.json()
            logger.info(f"Command Successful : {function_name}")
        except requests.exceptions.RequestException as e:
            logger.error(f"Unexpected Error : {str(e)}")
            raise Exception(f"Unexpected Error : {str(e)}")

    # def batch_command(self,
    #                   object_paths: List[str],
    #                   function_names: List[str],
    #                   parameters: List[Dict[str, Any]],
    #                   generate_transaction: bool = True) -> Dict[str, Any]:
    #     """
    #     Batch Remote Control Commands
    #     :param object_paths: List of Paths to UE Object
    #     :param function_names: List of Names of Function to Call
    #     :param parameters: List of Dictionaries of Parameters to Pass
    #     :param generate_transaction: Whether to Generate Transaction for Undo
    #     :return: Dictionary of Response JSON of /remote/batch
    #     """
    #     if not object_paths:
    #         raise ValueError("object_paths must not be empty.")
    #         if isinstance(parameters, list):
    #             if len(parameters) != len(object_paths):
    #                 raise ValueError("Length of parameters list must match length of object_paths")
    #             params_list = parameters
    #         else:
    #             params_list = [parameters for _ in object_paths]
    #
    #         logger.info(
    #             "Batch command -> %s for %d objects (generate_transaction=%s)",
    #             function_name, len(object_paths), generate_transaction
    #         )
    #
    #         requests_array = []
    #         for i, (obj_path, params) in enumerate(zip(object_paths, params_list), start=request_id_start):
    #             body = {
    #                 "objectPath": obj_path,
    #                 "functionName": function_name,
    #                 "parameters": params or {},
    #                 "generateTransaction": generate_transaction,
    #             }
    #             requests_array.append({
    #                 "RequestId": i,
    #                 "URL": "/remote/object/call",
    #                 "Verb": "PUT",
    #                 "Body": body,
    #             })
    #         payload = {"Requests": requests_array}
    #
    #         try:
    #             response = requests.put(self.batch_url, json=payload, headers=headers or {}, timeout=timeout)
    #             response.raise_for_status()
    #             result = response.json()
    #             logger.info("Batch command successful: %s (%d items)", function_name, len(object_paths))
    #             return result
    #
    #         except requests.exceptions.RequestException as e:
    #             logger.error("Batch command failed: %s", str(e))
    #             raise Exception(f"Batch command failed: {str(e)}")

    def find_actor_by_label(self, actor_label: str) -> Optional[str]:
        """
        Find an actor by its label and Return its Path
        :param actor_label: The Label of the Actor to Find
        :return: The Actor Path if Found, None Otherwise
        """
        try:
            # Get All Actors
            # TODO - What if we use another subsystem at runtime?
            actors_result = self.send_command(
                "/Script/UnrealEd.Default__EditorActorSubsystem",
                "GetAllLevelActors")
            # Get Return Value, otherwise return empty list
            actors = actors_result.get("ReturnValue", [])
            # Find the actor with the matching label
            for actor_path in actors:
                try:
                    label_result = self.send_command(actor_path, "GetActorLabel")
                    label = label_result.get("ReturnValue", "")
                    if label == actor_label:
                        return actor_path
                except Exception:
                    # Try to Check if the Actor Name in the Path Matches
                    if actor_label in actor_path:
                        return actor_path
            return None
        except Exception as e:
            logger.error(f"Error Finding Actor by Label : {str(e)}")
            return None

    def get_component_by_class(self, actor_path: str, component_class: str) -> Optional[str]:
        """
        Get Component by its Class from Actor
        :param actor_path: Path to Actor
        :param component_class: Class of Component to Find
        :return: Component Path if Found, None Otherwise
        """
        try:
            result = self.send_command(actor_path, "GetComponentByClass", {"ComponentClass": component_class})
            return result.get("ReturnValue")
        except Exception as e:
            logger.error(f"Error Getting Component : {str(e)}")
            return None

_unreal_connection = None
def get_unreal_connection():
    """ Get or Create Unreal Connection """
    global _unreal_connection

    if _unreal_connection is not None:
        try:
            if _unreal_connection.test_connection():
                return _unreal_connection
        except Exception as e:
            logger.warning(f"Existing Connection is No Longer Valid : {str(e)}")
            _unreal_connection = None

    _unreal_connection = UnrealConnection()
    if not _unreal_connection.test_connection():
        logger.error("Failed to Connect to Unreal Engine in get_unreal_connection()")
        _unreal_connection = None
        raise Exception("Could not connect to Unreal Engine. Make sure Remote Control API enabled.")
    logger.info("Created New Persistent Connection to Unreal Engine")

    return _unreal_connection