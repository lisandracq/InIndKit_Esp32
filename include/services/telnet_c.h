#include <Arduino.h>
#include "ESPTelnet.h"

class Telnet_c : public ESPTelnet
{

public:
  bool start(uint16_t server_port);
  bool start() { return (start(this->server_port)); }
  Telnet_c(uint16_t server_port) : ESPTelnet() { this->server_port = server_port; }
  template <typename T>
  void plot(const char *varName, T x, T y, const char *unit = NULL);
  template <typename T>
  void plot(const char *varName, T y, const char *unit = NULL);
  template <typename T>
  void print(const T &data);
  template <typename T>
  void print(const T &data, int base);
  void println();
  template <typename T>
  void println(const T &data);
  template <typename T>
  void println(const T &data, int base);
  void update(void);
  uint16_t serverPort() { return (this->server_port); }
};

bool Telnet_c::start(uint16_t server_port)
{
  if (this->connected)
    this->stop();
  this->server_port = server_port;
  onDisconnect([](String ip)
               {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" disconnected"); });
  onConnectionAttempt([](String ip)
                      {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" tried to connected"); });
  onReconnect([](String ip)
              {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" reconnected"); });

  return (this->begin(server_port));
}

void Telnet_c::update(void)
{
  this->loop();
  if (!connected && Serial.available() && on_input != NULL)
  {
    on_input(Serial.readStringUntil('\n'));
  }
}

template <typename T>
void Telnet_c::plot(const char *varName, T y, const char *unit)
{
  plot(varName, (T)millis(), y, unit);
}
template <typename T>
void Telnet_c::plot(const char *varName, T x, T y, const char *unit)
{
  print(">"); // Inicio de envio de dados para um gráfico.
  print(varName);
  print(":");
  print(x);
  print(":");
  print(y);
  if (unit != NULL)
  {
    print("§"); // Unidade na sequência
    print(unit);
  }
  println("|g"); // Modo Grafico
}

template <typename T>
void Telnet_c::print(const T &data)
{
  if (this->connected)
    this->print(data);
  else
    Serial.print(data);
}

template <typename T>
void Telnet_c::print(const T &data, int base)
{
  if (this->connected)
    this->print(data, base);
  else
    Serial.print(data, base);
}

template <typename T>
void Telnet_c::println(const T &data)
{
  if (this->connected)
    this->println(data);
  else
    Serial.println(data);
}

template <typename T>
void Telnet_c::println(const T &data, int base)
{
  if (this->connected)
    this->println(data, base);
  else
    Serial.println(data, base);
}

void Telnet_c::println()
{
  if (this->connected)
    this->println();
  else
    Serial.println();
}