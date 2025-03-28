import asyncio

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"Connected: {addr}")

    while True:
        data = await reader.read(1024)
        if not data:
            break
        print(f"Received from {addr}: {data.decode()}")
        writer.write(data)
        await writer.drain()

    print(f"Disconnected: {addr}")
    writer.close()
    await writer.wait_closed()

async def main():
    server = await asyncio.start_server(handle_client, "0.0.0.0", 8081)
    addr = server.sockets[0].getsockname()
    print(f"Serving on {addr}")

    async with server:
        await server.serve_forever()

asyncio.run(main())
