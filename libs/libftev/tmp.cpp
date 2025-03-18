
// #include <EventLoop.hpp>
// #include <EventLoop/BaseIOWatcher.hpp>
// #include <Format.hpp>
// #include <logger/Logger.hpp>
// #include <socket/AddrInfos.hpp>
// #include <socket/Socket.hpp>

// #include <cassert>
// #include <cstddef>
// #include <exception>
// #include <vector>

// namespace ftev {

// class BaseTransport {
// public:
//   virtual void close() = 0;
// };

// class ReadTransport : virtual public BaseTransport {
// public:
//   virtual std::size_t read(void *buf, std::size_t size) = 0;
// };

// class WriteTransport : virtual public BaseTransport {
// public:
//   virtual std::size_t write(void const *buf, std::size_t size) = 0;
// };

// class BaseProtocol {
// private:
//   class Handler : public ftev::EventLoop::BaseIOWatcher {
//   private:
//     BaseProtocol &_protocol;
//     void on_read() {
//       _protocol.on_read();
//     };
//     void on_write() {
//       _protocol.on_write();
//     };
//     void on_except() {
//       _protocol.on_except();
//     };
//   };

// protected:
//   Handler _handler;

// public:
//   virtual void on_read() {};
//   virtual void on_write() {};
//   virtual void on_except() {};
// };

// class ReadProtocol : virtual public BaseProtocol {
// public:
//   static std::size_t const _chank_size;

//   virtual ReadTransport &getReadTransport() = 0;

//   void on_read() {
//     ReadTransport &transport = getReadTransport();
//     std::vector<char> chank;
//     try {
//       chank.resize(_chank_size);
//       chank.resize(transport.read(chank.data(), chank.size()));
//     } catch (std::exception const &e) {
//       ftpp::logger.log(ftpp::Logger::WARN,
//                        ftpp::Format("ReadProtocol read: {}") % e.what());
//       return;
//     }
//     if (chank.empty()) {
//       EventLoop::BaseIOWatcher::event_t event =
//           _handler.get_events() & ~ftpp::BaseSelector::READ;
//       if (event)
//         _handler.modify(event);
//       else
//         _handler.stop();
//       on_eof();
//     } else {
//       on_data(chank);
//     }
//   }

//   virtual void on_data(std::vector<char> const &data) = 0;
//   virtual void on_eof() = 0;
// };

// class WriteProtocol : virtual public BaseProtocol {
// private:
//   std::vector<char> _buffer;
//   bool _draining : 1;

// public:
//   virtual WriteTransport &WriteTransport() = 0;

//   void on_write() {
//     assert(!_buffer.empty());
//     WriteTransport &transport = WriteTransport();
//     try {
//       std::size_t size = transport.write(_buffer.data(), _buffer.size());
//       _buffer.erase(_buffer.begin(), _buffer.begin() + size);
//     } catch (std::exception const &e) {
//       ftpp::logger.log(ftpp::Logger::WARN,
//                        ftpp::Format("WriteProtocol write: {}") % e.what());
//       return;
//     }
//     if (_buffer.empty()) {
//       EventLoop::BaseIOWatcher::event_t event =
//           _handler.get_events() & ~ftpp::BaseSelector::WRITE;
//       if (event)
//         _handler.modify(event);
//       else
//         _handler.stop();
//       if (_draining) {
//         on_drain();
//         _draining = false;
//       }
//     }
//   }

//   void write(char const *buffer, std::size_t size) {
//     assert(!_draining);
//     if (!size)
//       return;
// #if defined(FT_SUBJECT_NOT_COMPLIANT)
//     if (_buffer.empty()) {
//       try {
//         try {
//           size_t written = _socket.write(buffer, size);
//           if (written == size)
//             return;
//           buffer += written;
//           size -= written;
//         } catch (ftpp::OSError const &e) {
//           switch (e.get_errno()) {
//           case EAGAIN:
//             break;
//           default:
//             throw;
//           }
//         }
//       } catch (std::exception const &e) {
//         ftpp::logger.log(ftpp::Logger::WARN,
//                          ftpp::Format("TCPConnection write: {}") % e.what());
//       }
//     }
// #endif
//     // I was aiming for a strong server.
//     // I could not think of a countermeasure for lack of resources here.
//     // The server terminates with a THROW in the event loop.
//     _buffer.insert(_buffer.end(), buffer, buffer + size);
//     // ↑↑↑
//     if (is_active()) {
//       event_t event = get_events();
//       if (!(event & ftpp::BaseSelector::WRITE))
//         modify(event | ftpp::BaseSelector::WRITE);
//     } else
//       start(_socket.getSockfd(), ftpp::BaseSelector::WRITE);
//   }
//   virtual void on_drain() = 0;
// };

// } // namespace ftev