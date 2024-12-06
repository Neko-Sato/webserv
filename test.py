#!/usr/bin/env python3
import asyncio
import sys

async def async_stdio() -> tuple[asyncio.StreamReader, asyncio.StreamWriter]:
  loop = asyncio.get_running_loop()
  stdin_reader = asyncio.StreamReader()
  stdin_protocol = asyncio.StreamReaderProtocol(stdin_reader)
  await loop.connect_read_pipe(lambda: stdin_protocol, sys.stdin)
  stdout_transport, stdout_protocol = await loop.connect_write_pipe(
    asyncio.streams.FlowControlMixin, sys.stdout
  )
  stdout_writer = asyncio.StreamWriter(stdout_transport, stdout_protocol, stdin_reader, loop)
  return stdin_reader, stdout_writer

async def main():
  stdin, stdout = await async_stdio()
  reader, writer = await asyncio.open_connection("localhost", 8080)
  async def display(reader: asyncio.StreamReader):
    while True:
      data = await reader.readline()
      if not data:
       break
      stdout.write("receive: ".encode() + data)
  read_task = asyncio.create_task(display(reader))
  while True:
    msg = await stdin.readline()
    if not msg:
      break
    writer.write(msg)
  await writer.drain()
  writer.close()
  await writer.wait_closed()
  await read_task

if __name__ == "__main__":
  asyncio.run(main())