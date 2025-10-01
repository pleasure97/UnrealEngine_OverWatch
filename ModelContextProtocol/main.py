import logging
from contextlib import asynccontextmanager
from mcp.server.fastmcp import FastMCP
from typing import AsyncIterator, Dict, Any
from unreal_connection import get_unreal_connection

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


def main():
    print("Hello from modelcontextprotocol!")


if __name__ == "__main__":
    main()
