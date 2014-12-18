#include <iostream>
#include <sstream>
#include <unistd.h>

#include "../include/include.h"
#include "../include/common_functions.cpp"
#include "player.h"

#ifdef __QT__
#include <QApplication>
#include <QThread>
#include "widget.h"
#include "log.h"
#include "../include/updater.h"
#endif

// #define _sock_ ((clients.at(inner_number)).get_socket())  //this is socket to send messages to the particular client
#define _id_ (clients.at(inner_number).id)                   //what ? //inner_number -- number of element in clients vector, everywhere.
// #define _sock_ (clients.socket.at(inner_number))          //this is socket to send messages to the particular client
// #define _id_ (clients.id.at(inner_number))                //what ? //inner_number -- number of element in clients vector, everywhere.

// using boost::asio::ip::tcp;
using namespace std;

std::vector<player> clients;

#ifdef __QT__
Updater updater;//class to update log window via QApplication::connect
#endif

std::mutex clients_mutex;        // mutex to avoid undefined behavior with "clients".        some threads can read\write into it simultaniously.

void test_message_sender(int inner_number = 0, std::string message = "", bool test = true);
void output (std::string string, bool with_new_line = true);
void message_sender(int inner_number, std::string message); //send message to the client
void session(int inner_number);
void server(boost::asio::io_service &io_service, unsigned short port);
void deck_distribution();
// void iffunction(int inner_number, std::string &input_message);

//output to cout and to log window
void output (std::string string, bool with_new_line)
{
  if (with_new_line)
    std::cout << string << std::endl;
  else
    std::cout << string << ";\t";
  #ifdef __QT__
  updater.send_message_slot(QString::fromStdString(string));
  #endif
}

void handler_action(int signal_number) //this function will be called when server is killed or receive certain SIGnals
{
  for (auto iterator = clients.begin(); iterator != clients.end(); ++iterator)
    if ((*iterator).connected)
      message_sender(iterator - clients.begin(), make_message(0, "service", "serverwaskilled"));
    
  std::cout << std::endl << "Server was killed with signal " << signal_number << ". Have a nice day !" << std::endl << std::endl;
  exit(10);
}

void death_handler()  //this function defines what SIGnals we should handle
{ 
  struct sigaction action;
  action.sa_handler = handler_action; /* Do our own action */
  sigaction(SIGTERM, &action, NULL);  /* killall */
  sigaction(SIGINT,  &action, NULL);  /* ctrl-c  */
}

//divide message into parts and decide what to do with the message
void iffunction(int inner_number, std::string &input_message)
{
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  if (input_message.size() < _ID_LENGTH_ + _HEAD_LENGTH_)
    return;
  std::stringstream ss;
  std::string message = input_message;
  
  int block_number = (input_message.size() - _ID_LENGTH_ - _HEAD_LENGTH_) / _BLOCK_LENGTH_ + 2; 
  std::vector<std::string> block;
  block.resize(block_number);
  
  block.at(0) = get_client_id(message);
  block.at(1) = get_head(message);
  for ( int i = 2; i < block_number; i++)
  {
    block.at(i) = get_block(message, i);
    output(block.at(i));
  }
  
  std::lock_guard<std::mutex> iffunction_lock(clients_mutex);  
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
  if ( block.at(1) == "alive" )
  {
    message_sender(inner_number, message);    //     send him back alive signal
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
//     send to others that he is connected
  }

  if ( block.at(1) == "statistics" )  { /* send him statistics */ }
  if ( block.at(1) == "trade"      )  {  }
  if ( block.at(1) == "game"       )  {  }
  if ( block.at(1) == "offer"      )  {  }
  if ( block.at(1) == "chat"       )
  {
    ss.str("");
    ss << "player " << get_client_id(message) << "[" << ctiming() << "]: " << get_chat_message(message);
    output(ss.str());
    for (auto iterator = clients.begin(); iterator != clients.end(); ++iterator)
    {
      if ((*iterator).connected)
      {
        ss.str("");
        ss << make_client_id(get_int_client_id(message)) << make_head("chat") << "player " << get_client_id(message) << "[" << ctiming() << "]: " << get_chat_message(message); //here should be some changes somewhen: change player N to client.name or something
        message_sender(iterator - clients.begin(), ss.str());
      }
    }    //then block[2] is playername
  }
  
  if ( block.at(1) == "service" )
  {
    std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
    if( block.at(2) == "clientwaskilled" )
    {
      std::cout << "Client # " << block.at(0) << " was killed. Let's forget about him..." << std::endl;
      std::cout << "inner_number = " << inner_number << ";\tclients.size() = " << clients.size() << std::endl;
      clients.at(inner_number).connected = false;
      clients.at(inner_number).playing   = false;
      std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
    }
  }
  
  clients_mutex.unlock();
  std::cout << "iffunction __LINE__ = " << __LINE__ << std::endl;
}

//probably useless...
/*
void session(int inner_number)
{
//   std::cout << "session __LINE__ = " << __LINE__ << std::endl;
  std::cout << "inner_number = " << inner_number << std::endl;
  std::unique_lock<std::mutex> session_lock(clients_mutex, std::defer_lock);
//   int client_id = clients.id.at(inner_number);
//   session_lock.unlock();
  std::stringstream ss;
  std::string message;
  ss.str("");
  try
  {
    for (;;) 
    {
      char data[max_buffer_length] = {};
      boost::system::error_code error;
      session_lock.lock();
      _sock_.read_some(boost::asio::buffer(data), error); //read message
      session_lock.unlock();
      if (error == boost::asio::error::eof)
      {
        std::cout << "session: break is here" << std::endl;
        break; // Connection closed cleanly by peer.
      }
      else if (error)
      {
        std::cout << "session: some other error" << std::endl;
        throw boost::system::system_error(error); // Some other error.
      }
         
      ss.str("");      ss << "The original message was \"" << data << "\"";  std::cout << ss.str().c_str() << std::endl;
      message = data;
      #ifdef __QT__
      updater.send_message_slot(QString::fromStdString(message));
      #endif
      std::strcpy (data, ss.str().c_str());
      iffunction(inner_number, message);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in session: " << e.what() << "\n";
    session_lock.unlock();
  }
}*/
//send messages from cin directly
/*
void test_message_sender(int inner_number, std::string message, bool test)
{
//   std::cout << "test_sender __LINE__ = " << __LINE__ << std::endl;
  std::stringstream ss;
  int check_time = -1;
  int counter = 0;
  for (;;)
  {
    try
    {
      if (test)
      {
        std::cout << "Enter inner number... " ;
        std::cin  >> inner_number;
        std::cin.clear();
        
        if (check_time == time(NULL))
        {
          if ( counter > 10 )
          {
            std::cout << "Sender happly died..." << std::endl;
            break;  //kill it if cycle goes wild
          }
        }
        else
        {
            counter = 0;
        }
        check_time = time(NULL);
        counter++;
        
        srand (time(NULL));  // initialize random seed 
        ss.str(""); 
//         ss << (rand() % 80000 + 10000); //rand from 10000 to 90000
        for(int i = 0; i < _ID_LENGTH_ + _HEAD_LENGTH_ + 1; i++) 
          ss << (rand() % 9); //rand from 0 to 9
        message = ss.str();
      }
      clients_mutex.lock();
      clients.write(message)
      clients_mutex.unlock();
      std::cout << "Message \"" << message << "\" succesfully sent to client with id " << clients.at(inner_number).id << std::endl;
    }
    catch (std::exception& e)
    {
      std::cout << "test_sender __LINE__ = " << __LINE__ << std::endl;
      std::cerr << "Exception in cin thread: " << e.what() << "\n";
      clients_mutex.unlock();
    }
  }
}
*/
//send message to the client
void message_sender(int inner_number, std::string message)
{  
  /*      //probably we don't need mutex here, because ..... stop, why ? because mutex will be out there
  char data[max_buffer_length] = {};      
  try
  {
    std::strcpy (data, message.c_str());
    boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
    std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.at(inner_number).id << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in message sender: " << e.what() << "\n";
  }*/
  clients.at(inner_number).write(message);
}

//send message and wait for the answer. dangerous, will be fixed. can wait forever. 
std::string send_receive(int inner_number, std::string message)
{  
  /*
  char data[max_buffer_length] = {};      
  try
  {
    boost::system::error_code error;
    std::strcpy (data, message.c_str());
    boost::asio::write(_sock_, boost::asio::buffer(data, max_buffer_length));    //send message back to client
//     std::cout << "Message \"" << data << "\" succesfully sent to client with id " << clients.id.at(inner_number) << std::endl;
    _sock_->read_some(boost::asio::buffer(data), error); //read message
//     _sock_.read_some(boost::asio::buffer(data), error); //read message
    message = data;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in send_receive: " << e.what() << "\n";
    return "exception";
  }*/
  return clients.at(inner_number).send_receive(message);
}

void current_bribe(){}

//send cards to the clients
void deck_distribution()
{
//   std::cout << "deck_distribution __LINE__ = " << __LINE__ << std::endl;
  int count_players = 0;
  for (auto playing_client = clients.begin(); playing_client != clients.end(); ++playing_client)
    if ((*playing_client).playing)
      count_players++;
    
  if(count_players != 3 && count_players != 4)
  {
    std::cout << "distribution: wrong amount of players " << std::endl;
    return;
  }
  
  std::stringstream ss;
  std::vector<card> deck = create_deck();
  
  std::srand(time(0));  //without this line random_shuffle will always return the same value

  for (int i = 0; i < rand() % 10 + 2; i++)
    std::random_shuffle(deck.begin(), deck.end());  //shuffle well
  
  std::unique_lock<std::mutex> deck_distribution_lock_clients(clients_mutex, std::defer_lock);
  
  std::vector<std::vector<card>> d_player;  // 3 players with vector of cards
  std::vector<card> talon;
  d_player.resize(3);
  
  
  int talon_place = rand() % 20 + 6;
  talon_place = talon_place + talon_place % 2; //make it even(!)
  
  for ( int i = 0; i < 32; )
  {   //deck
    for (auto i_d_player = d_player.begin(); i_d_player != d_player.end(); ++i_d_player)
    { //2 cards to player and may be to talon
//       std::cout << i_d_player - d_player.begin() << std::endl;
      (*i_d_player).push_back(deck.at(i++));  //into the hand ; (*i_d_player) is vector of cards
      (*i_d_player).push_back(deck.at(i++));  //into the hand ; (*i_d_player) is vector of cards
      if (i == talon_place)
      {
        talon.push_back(deck.at(i++));
        talon.push_back(deck.at(i++));
      }
    }
  }
  
  deck_distribution_lock_clients.lock();
  auto i_d_player = d_player.begin();  //(*i_d_player) is vector of cards
  for (auto playing_client = clients.begin(); playing_client != clients.end(); ++playing_client)
  {
    if ((*playing_client).playing == false)
      continue;
    
    ss.str(""); ss << "\nd_player.at(" << i_d_player - d_player.begin() << "):"; output(ss.str());  //(*i_d_player) is vector of cards
    for (auto it = (*i_d_player).begin(); it != (*i_d_player).end(); ++it)  //(*i_d_player) is vector of cards (10)
    {
      output((*it).name, false);
      ss.str(""); ss << make_client_id((*playing_client).id) << make_head("distribution") << make_block((*it).number);
      message_sender(playing_client - clients.begin(), ss.str()); //send cards to the players
    }
    ++i_d_player;
  }
  deck_distribution_lock_clients.unlock();
  
  output("\ntalon:"); 
  for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name, false);
  /*
  output("pl1:");     ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl1.begin(); it != pl1.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl2:");   ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl2.begin(); it != pl2.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\npl3:");   ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).suit;  output(ss.str().c_str());
                      ss.str(""); for (auto it = pl3.begin(); it != pl3.end(); ++it)     ss << (*it).value; output(ss.str().c_str());
  output("\ntalon:"); ss.str(""); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);    * /
  rearrangement(std::ref(pl1));
  rearrangement(std::ref(pl2));
  rearrangement(std::ref(pl3));
  
  output("\n\nRearrangemented:\n\n");  
  output("pl1:");     for (auto it = pl1.begin(); it != pl1.end(); ++it)     output((*it).name);
  output("\npl2:");   for (auto it = pl2.begin(); it != pl2.end(); ++it)     output((*it).name);
  output("\npl3:");   for (auto it = pl3.begin(); it != pl3.end(); ++it)     output((*it).name);
  output("\ntalon:"); for (auto it = talon.begin(); it != talon.end(); ++it) output((*it).name);
  */
  
}

//check if certain client is alive
std::string check_one(int inner_numer)
{
  std::lock_guard<std::mutex> check_one_lock(clients_mutex);
  return send_receive(inner_numer, make_message(inner_numer, "alive"));
}


//check all active players
std::vector<int> check_alive()
{
  std::vector<int> check;
  std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
  std::string message;
  std::lock_guard<std::mutex> check_alive_clients_lock(clients_mutex);
  for (auto playing_client = clients.begin(); playing_client != clients.end(); ++playing_client)
  {
    if ((*playing_client).playing == false)
      continue;
    message = send_receive(playing_client - clients.begin(), make_message((*playing_client).id, "alive"));
    std::cout << "check_alive __LINE__ = " << __LINE__ << std::endl;
    if (message != make_message((*playing_client).id, "alive"))
      check.push_back(playing_client - clients.begin());
  }
  
  return check;
}

int check_active(int number = 3)
{
  int count_players = 0;
  for (auto playing_client = clients.begin(); playing_client != clients.end(); ++playing_client)
    if ((*playing_client).playing)
      count_players++;
    
  return number - count_players;
}

int trade(int first, int &minimum_bet)   // return the number of player who made the booking or -1 if there is raspass
{   // minimum_bet or minimum_bet + 1  ?
  std::cout << "trade    __LINE__ = " << __LINE__ << std::endl; 
  std::unique_lock<std::mutex> trade_clients_lock(clients_mutex, std::defer_lock);  //clever lock
  std::string message;
  
  int bet_number = 0;
  int counter = 0;
  
  trade_clients_lock.lock();
  for (auto it = clients.begin(); it != clients.end(); ++it)
    if ((*it).connected)
      (*it).bet = -1;  //reset them all
      
  auto playing_client = clients.begin();
  for (playing_client = clients.begin(); playing_client != clients.end(); playing_client++)
  { 
    if ((*playing_client).playing == false)
      continue;
    if (counter == first) // we shall start trade from this place
      break;
    counter++;
  }
  trade_clients_lock.unlock();
  
  while(true)
  { //cycle from the particular place in the vector until the end of the trading
    trade_clients_lock.lock();
    if (playing_client == clients.end())
    {
      playing_client = clients.begin();
      trade_clients_lock.unlock();
      continue;
    }
    if ((*playing_client).playing == false)
      continue;

    //begin counting "pass"es
    bet_number = 0;
    for (auto iter = clients.begin(); iter != clients.end(); ++iter)  
      if ((*iter).bet != 0) //check how many people have bets; need to be 0 or 1 to end the trade
        bet_number++;
      
    if (bet_number == 0)      /*raspass*/
    {
      trade_clients_lock.unlock();
      return -1;              /*raspass*/
    }
    else if (bet_number == 1) //booking was made or there were 2 passes; 
    {
      for (auto iter = clients.begin(); iter != clients.end(); ++iter)  
        if ((*iter).bet > 0)
        {
          trade_clients_lock.unlock();
          return (*iter).id; // number of player who made the booking;
        }
    }//end counting "pass"es
      
    if ((*playing_client).bet == 0) // "pass" -> 0 ; bet > 0; no decision -> -1
    {
      playing_client++;    
      trade_clients_lock.unlock();
      continue;
    }
    
    trade_another_try: //in case of exception in send_receive try another time (see below)
    message = make_block("minimumbet") + make_block(minimum_bet);
    message_sender        (playing_client - clients.begin(), make_message((*playing_client).id, "trade", message));
    message = send_receive(playing_client - clients.begin(), make_message((*playing_client).id, "trade", "yourturn"));  //I expect bet in the reply
    //here must be some check if the client is alive (see below) and here must be some check to kill the waiting after some time...probably
    if (message == "exception") //in case of exception in send_receive try again
    {
      trade_clients_lock.unlock();
      std::cerr << "exception trade __LINE__ " << __LINE__  << std::endl;
      usleep(5e5);
      goto trade_another_try;
    }
    
    for (auto it = clients.begin(); it != clients.end(); ++it)
      if ((*it).connected)
        message_sender(it - clients.begin(), message);//send received message to everybody
      
    (*playing_client).bet = atoi((get_block(message)).c_str());
    if (minimum_bet < (*playing_client).bet)
      minimum_bet = (*playing_client).bet;
    
    playing_client++;
    trade_clients_lock.unlock();
  }
}

void show_talon() {std::cout << "show_talon  __LINE__ = " << __LINE__ << std::endl; usleep(0e6);}

int booking(int trade_winner, int minimum_bet)
{
//   std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  if (trade_winner == -1) /*raspass*/
    return -1;
  
  std::unique_lock<std::mutex> booking_clients_lock(clients_mutex, std::defer_lock);  //clever lock
  booking_clients_lock.lock();
  int inner_number_trade_winner;
  for (auto it = clients.begin(); it != clients.end(); ++it)
    if ((*it).playing)
      if((*it).id == trade_winner)
        inner_number_trade_winner = it - clients.begin();
  
  for (auto it = clients.begin(); it != clients.end(); ++it)
    if ((*it).connected)
      message_sender(it - clients.begin(), make_message(trade_winner, "booking", minimum_bet));  //tell everybody who is booking
  std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  std::string message;
  std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  
  booking_another_try:
  message = send_receive(inner_number_trade_winner, make_message(trade_winner, "booking", "decision"));  //I expect bet in the reply
  std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  //here must be some check if the client is alive (see below) and here must be some check to kill the waiting after some time...probably
  if (message == "exception")
  {
      std::cerr << "exception booking __LINE__ " << __LINE__  << std::endl;
      usleep(5e5);
      goto booking_another_try;
  }
  std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  for (auto it = clients.begin(); it != clients.end(); ++it)
    if ((*it).connected)
      message_sender(it - clients.begin(), message);  //tell everybody what was booked finally
      
  std::cout << "booking  __LINE__ = " << __LINE__ << std::endl;
  
  if (get_block(message) == "withoutthree")
  { //don't do calculations here. we should do them in the more appropriate place
    std::cout << "booking; player " << clients.at(trade_winner).id << " went without three from bet " << minimum_bet << std::endl;
    booking_clients_lock.unlock();
    return -3;
  }
  booking_clients_lock.unlock();

  std::cout << "booking; atoi( get_block(message).c_str() ) = " << atoi( get_block(message).c_str() ) << std::endl;
  return atoi( get_block(message).c_str() ); //this should be a final bet (int > 0)
  
}
void vists()      {std::cout << "vists    __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void moves()      {std::cout << "moves    __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void offers()     {std::cout << "offers   __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void result()     {std::cout << "result   __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}
void cleaning()   {std::cout << "cleaning __LINE__ = " << __LINE__ << std::endl; usleep(5e6);}

//main game cycle
void game_cycle()
{
//   std::cout << "game_cycle __LINE__ = " << __LINE__ << std::endl;
  std::vector<int> check;
  std::stringstream ss;
  usleep(5e6);
  try
  {
    while (check_active() != 0)
    {
      ss.str("");  ss << "waiting for " << check_active() << " more clients to play";  output (ss.str());
      usleep(5e6);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in game_cycle: " << e.what() << "\n";
  }
  
  int number_of_players = 3;
  int trade_first = 0; // = rand() % number_of_players; //who will trade first
  int trade_winner;
  int minimum_bet = -1;
  int final_bet;
  
  while(true)
  {
    do
    {
      check.resize(0);
      check = check_alive();
      for (auto iterator : check)
      {
        ss.str("");  ss << "waiting for client " << iterator << " to connect";  output (ss.str());
      }
      if (check.size() > 0)
      {
        ss.str("");  ss << "waiting for " << check.size() << " more clients " << " to connect";  output (ss.str());
        usleep(5e6);
      }
    } while ( check.size() != 0 );
    
    final_bet = -1;
    trade_winner = -1;
    
    deck_distribution();                            //should return talon somehow
    trade_winner = trade(trade_first, minimum_bet); //done
    if (trade_winner != -1)                         //if somebode booked the game
      show_talon();                                 // not done
    final_bet = booking(trade_winner, minimum_bet); //simple
    minimum_bet = -1; /* ????? */
    
    if (final_bet == -3)                            //skip the moves
    {
      result();  //god knows what we should pass there...
      cleaning();
      if (trade_first == number_of_players - 1) 
        trade_first = 0; //who will trade first
      else
        trade_first++;   //who will trade first
      continue;
    }
      
    if (final_bet > 0)  // if (final_bet < 0) -> raspass
    {
      vists();
    }
    
    //offers.detach
    moves();
    offers();
    //offers.join
    
    result(); //god knows what we should pass there...
    cleaning();
    
    if (trade_first == number_of_players - 1) trade_first = 0; //who will trade first
    else                                      trade_first++;   //who will trade first
  }
  
}


/*
// void output (TYPE data, Updater &updater)
// {
//   std::cout << data << std::endl;
//   stringstream ss; ss << data;
//   updater.send_message_slot(QString::fromStdString(ss.str()));
// }*/

//server can receive and process some certain messages, e.g. chat, alive, active...
void server(boost::asio::io_service &io_service, unsigned short port)
{
//   std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  std::cout << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "\tServer is started on port " << port             << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
            << std::endl;
  std::string message;
  std::string chat_message;
  std::stringstream ss;
  std::unique_lock<std::mutex> server_lock(clients_mutex, std::defer_lock);
  bool reconnected;
  int id;
  int inner_number;
  std::cout << "server __LINE__ = " << __LINE__ << std::endl;  
//   deck_distribution();
  std::cout << "port = " << port << std::endl;
  boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
//   std::thread([]{ test_message_sender();}).detach(); //cin sender
  std::thread(game_cycle).detach(); 
  for (;;)
  {
    boost::asio::ip::tcp::socket sock(io_service);    //open socket
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    acceptor.accept(sock);           //wait for message
    
    char data[max_buffer_length] = {};
    boost::system::error_code error;
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    sock.read_some(boost::asio::buffer(data), error); //read message
//     size_t reply_length = boost::asio::read(sock,boost::asio::buffer(data, max_buffer_length));
    message = data;
    if (message.size() < _ID_LENGTH_ + _HEAD_LENGTH_)      
    {
      std::cerr << "Error: server received short message: \"" << message << "\" of size " << message.size() << std::endl;
      continue;
    }
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
    
    if (get_head(message) == "alive")
    {
      reconnected = false;
      id = get_int_client_id(message);
      std::cout << "server (got alive) __LINE__ = " << __LINE__ << std::endl;
      server_lock.lock();
      for (auto iterator = clients.begin(); iterator != clients.end(); ++iterator)
      {
        if (id == (*iterator).id)
        {
          ss.str(""); ss << "Client # " << id << " reconnected"; output(ss.str());
          reconnected = true;
          inner_number = iterator - clients.begin();
          clients.at(inner_number).reconnect(std::move(sock));
          clients.at(inner_number).playing = true; /* !! TEMPORARY !! */
        }
      }

      if (!reconnected)
      {
        clients.resize(clients.size() + 1);
        inner_number = clients.size() - 1;
        std::cout << "server (got alive) __LINE__ = " << __LINE__ << std::endl;
        clients.at(inner_number).connect(std::move(sock), id);
        clients.at(inner_number).playing = true; /* !! TEMPORARY !! */
      }
      server_lock.unlock();
    }
    else
    {
      std::cout << "server __LINE__ = " << __LINE__ << std::endl;
      std::cout << "get_int_client_id(message) = " << get_int_client_id(message) << ";\tget_head(message) = " << get_head(message) << ";\tmessage = " << message << std::endl;
      iffunction(inner_number, message);
    }
    
    std::cout << "server __LINE__ = " << __LINE__ << std::endl;
  }
}

int main(int argc, char *argv[])
{
  death_handler();
  #ifdef __QT__
  QApplication a(argc, argv);
  Widget w;
  //w.show();
  Log log;
  
  QObject::connect(&updater, SIGNAL(send_message_signal(QString)), &log, SLOT(receive_message(QString)));
  #endif
  
  /*
//   Log log;
//   QTextEdit *textBox = log.findChild<QTextEdit *>("textEdit");  
//   log.show();
//   for(int i = 0; i < 10; i++  )
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//   }
//   log.show();
//   sleep(2);
//   for(int i = 11; i < 20; i++)
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//       log.repaint();
// //       sleep(1);
//   } */
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: ./_server <port>\n";
      return 1;
    }
    #ifdef __QT__
    log.show();
    #endif
    boost::asio::io_service io_service;
    //start the server
    std::thread server_thread(server, boost::ref(io_service), std::atoi(argv[1]));//.detach();
    #ifdef __QT__
    a.exec(); //oh god this is mandatory. nobody told me that this shit is called main thread. stupid qt, i hate it. at last this works now.
    #endif
    server_thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in server: " << e.what() << "\n";
  }
  
  return 0;
}

