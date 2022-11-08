#ifndef SERVER_HPP
# define SERVER_HPP

class server
{
  public:
    server();
    server(std::string port, std::string pwd);
    ~server();

  private:

    std::string   _port;
    std::string   _pwd;
    std::string   _host;

    int   _setUp(void);


};

#endif