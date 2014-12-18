
#define _CLIENT_

// #include <signal.h>
#include "../include/include.h"
#include "../include/common_functions.cpp"

#define _sock_ (socket_for_read.at(0))

int int_client_id;
// std::vector <boost::asio::ip::tcp::socket> socket_for_read;
char* host;   //these are globals because they are passed to the death handler
char* port;

// std::string make_head(std::string head);
// std::string make_client_id(int int_client_id);

void handler_action(int signal_number) //this function will be called when client is killed or receive certain SIGnals
{
  std::string message;
  std::stringstream ss;
  char request_char[max_buffer_length];
  std::strcpy( request_char, (make_message(int_client_id, "service", "clientwaskilled")).c_str() );
  try
  {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::connect(socket, resolver.resolve({host, port}));
    boost::asio::write(socket, boost::asio::buffer(request_char, max_buffer_length));    //send message back to client
    std::cout << std::endl << "Client was killed. Bye !" << std::endl << std::endl;
  }
  catch (std::exception& e) 
  {
    std::cout << std::endl << "Client was killed and server is unreacheable. Bye !" << std::endl << std::endl;
  }
  exit(10);
}

void death_handler()  //this function defines what SIGnals we should handle
{ 
  struct sigaction action;
  action.sa_handler = handler_action; /* Do our own action */
  sigaction(SIGTERM, &action, NULL);  /* killall */
  sigaction(SIGINT,  &action, NULL);  /* ctrl-c  */
}

/*
void cin_sender() //send message to server directly from cin
{
  std::string message;
  try
  {
    for (;;)
    {
      char data[max_buffer_length] = {};

      std::cout << "Feel free to enter message here...Format is: client_id" << _EMPTY_SYMBOL_ << "head" << _EMPTY_SYMBOL_ << "anything else. _EMPTY_SYMBOL_ = " << _EMPTY_SYMBOL_ << " ; _ID_LENGTH_ = " << _ID_LENGTH_ << " ; _HEAD_LENGTH_ = " << _HEAD_LENGTH_ << std::endl;
      
      getline ( std::cin, message );
      std::cin.clear();
      std::strcpy (data, message.c_str());
      boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message succesfully sent ! " << std::endl;
//       std::cout << "__LINE__ = " << __LINE__ << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in cin thread: " << e.what() << "\n";
  }
}*/

//send message to server directly from cin
void chat_sender(char* host, char* port)
{
  std::string message;
  std::stringstream ss;
  try
  {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    for (;;)
    {
      char data[max_buffer_length] = {};
      std::cout << "Feel free to enter message here..." << std::endl;
      getline ( std::cin, message );
      std::cin.clear();

      ss.str("");
      ss 
      << make_client_id(int_client_id) 
      << make_head("chat") 
      << make_block(message);
      std::strcpy (data, ss.str().c_str());
      boost::asio::ip::tcp::socket socket(io_service);
      boost::asio::connect(socket, resolver.resolve({host, port}));
      boost::asio::write(socket, boost::asio::buffer(data, max_buffer_length));    //send message back to client
      std::cout << "Message succesfully sent ! " << std::endl;
      socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
      socket.close();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in chat_sender: " << e.what() << "\n";
  }
}


std::string command_sender(std::string type = "")
{
  std::string message;
  std::cout << "Enter " << type << " command here..." << std::endl;
  getline ( std::cin, message );
  std::cin.clear();

  return message;
}

int client(char* host, char* port, int int_client_id)
{
//   std::cout << "client __LINE__ = " << __LINE__ << std::endl;
  std::stringstream stringstream;
  std::vector <boost::asio::ip::tcp::socket> socket_for_read;
  try
  {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::connect(socket, resolver.resolve({host, port}));
    socket_for_read.push_back(std::move(socket));
    std::cout << std::endl << "Successfully connected !" << std::endl;
    
    int minimum_bet = -1;

    char reply_char[max_buffer_length];      
    char request_char[max_buffer_length];
    std::string reply;
    std::string message;
    std::strcpy( request_char, (make_message(int_client_id, "alive")).c_str() );
//     std::cout << "request_char = \""<< request_char << "\"" << std::endl;
    boost::asio::write(_sock_, boost::asio::buffer(request_char, (size_t)std::strlen(request_char)));
    
//     std::thread ([host, port]{chat_sender(host, port);}).detach(); 
//     std::thread ([socket_for_read]{command_sender(socket_for_read);}).detach(); 
    while(true)
    {
      boost::asio::read(_sock_,boost::asio::buffer(reply_char, max_buffer_length));  //don't put this into try; it's lethal
      reply = reply_char;
      try
      {
        if ( get_head(reply) == "chat" )
        {
          std::cout << get_chat_message(reply) << std::endl;
        }
        else if ( get_head (reply) == "alive" )
        {
          boost::asio::write(_sock_, boost::asio::buffer(reply_char, (size_t)std::strlen(reply_char)));
        }
        else if ( get_head (reply) == "trade" )
        {
          std::cout << "Server sent trade: \"" << reply_char << "\"" << std::endl;
          if (get_block (reply) == "minimumbet")
          {
            minimum_bet = atoi((get_block(reply, 3)).c_str());
            std::cout << "minimumbet is " << minimum_bet << std::endl;
          }
          else if (get_block (reply) == "yourturn")
          {
            message = command_sender("trade");
            std::strcpy( request_char, (make_message(int_client_id, "trade", message)).c_str() );
            boost::asio::write(_sock_, boost::asio::buffer(request_char, (size_t)std::strlen(request_char)));
            std::cout << "Sent: " << request_char << std::endl;
          }
        }
        else if ( get_head (reply) == "booking" )
        {
          if (get_int_client_id (reply) == int_client_id && get_block(reply) == "decision")
          {
            message = command_sender("booking");
            message = append_block(make_block("booking") , message); // to make [booking___][message]
            std::strcpy( request_char, (make_message(int_client_id, "booking", message)).c_str() );
            boost::asio::write(_sock_, boost::asio::buffer(request_char, (size_t)std::strlen(request_char)));
            std::cout << "Sent: " << request_char << std::endl;
          }
          if (get_block(reply) == "booking")
          {
            std::cout << "Server sent finish booking: \"" << reply_char << std::endl;
          }
          else 
          {
            std::cout << "Server sent start booking: \"" << reply_char << std::endl; 
          }
        }
        
        else if ( get_head (reply) == "service" )
        {
          if ( get_block (reply) == "serverwaskilled" )
          {
            std::cerr << std::endl << "Server said that he was killed. What should we do ? " << std::endl;
            _sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
            _sock_.close();
            return 10;
          }
        }
        else
        {
          std::cout << "Server sent: \"" << reply_char << "\"" << std::endl;
        }
      }
      catch (std::exception& e)
      {
        std::cerr << "Exception in while: " << e.what() << "\n";
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in client: \"" << e.what() << "\"\r";// << std::endl;
    return 1;
  }
  

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: ./_client <host> <port>\n";
    return 1;
  }
//   std::strcpy(argv[1], "localhost");
  host = argv[1];
  port = argv[2];
  srand (time(NULL));  // initialize random seed: 
//   int_client_id = 18; 
  int_client_id = rand() % 80 + 3 ; //range 3-32
  std::cout << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "\tClient # " << int_client_id << " is started " << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << std::endl;
  death_handler();
  int client_return;
  int attempt_counter = 0;
  std::cout << "Trying to connect to host \"" << host << "\" on port " << port << std::endl;
  while (true)
  {
    client_return = client(host, port, int_client_id);
    if (client_return != 0)
    {
      usleep(6e6);
      std::cout << "Trying to reconnect to host " << host << " on port " << port << "; Attempt # " << ++attempt_counter << "...\t" ;//std::endl;
      continue;
    }
    break;
  }
  return 0;
}
