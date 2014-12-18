#include "player.h"

player::player()
{
  id = -1;
  bet = -1;
  connected = false;
  playing   = false;

  vister    = false;
  bidder    = false;
  
  mountain      = 0;
  pool          = 0;
  vist_up       = 0;
  vist_down     = 0;
  vist_opposite = 0;  //for 4
}

void player::reconnect(boost::asio::ip::tcp::socket sock)
{
  boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(std::move(sock));
  this->socket = socket;
  this->connected = true;
}  
  
void player::connect(boost::asio::ip::tcp::socket sock)
{
  boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(std::move(sock));
  this->socket = socket;
  this->connected = true;
}

void player::connect  (boost::asio::ip::tcp::socket sock, int id)
{
  boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(std::move(sock));
  this->socket = socket;
  this->connected = true;
  this->id = id;
}

int player::write(std::string message)  // return the number of sent bytes
{
  char data[max_buffer_length] = {};
  int bytes = 0;
  try
  {
    std::strcpy (data, message.c_str());
    bytes = (this->socket->write_some(boost::asio::buffer(data, max_buffer_length)));    //send message // return the number of sent bytes
  }
  catch (std::exception& e)
  {
    bytes = -50;
  }
  return bytes; // return the number of sent bytes
}

std::string player::read()
{
  char data[max_buffer_length] = {};
  try
  {
    this->socket->read_some(boost::asio::buffer(data)); //read message
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in read: " << e.what() << "\n";
    return "exception";
  }
  return (std::string) data;
}

std::string player::send_receive(std::string message)
{
  int bytes = 0;
  bytes = write(message); // return the number of sent bytes
  if (bytes == -50)
    return "exception";
  return read();
}


// player::socket()
// {
//     boost::asio::ip::tcp::socket socket(boost::asio::ip::tcp::socket);
//     return socket;
// }
