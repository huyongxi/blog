import asyncio
import websockets

async def handler(websocket):
    try:
        async for message in websocket:
            print(f"Received: {message}")
            response = f"Server received: {message}"
            await websocket.send("OK")
            print(f"Sent: {response}")
    except websockets.exceptions.ConnectionClosed:
        print("Connection closed")

async def main():
    async with websockets.serve(handler, "localhost", 7654):
        await asyncio.Future()  # 永久运行

if __name__ == "__main__":
    asyncio.run(main())
