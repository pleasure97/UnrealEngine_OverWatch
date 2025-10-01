import logging
import requests

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
                "objectpath": "/Script/UnrealEd.Default_EditorActorSubsystem",
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