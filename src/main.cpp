#include "naviguesser.hpp"
#include "nuscape.hpp"
int main(int argc, char **argv)
{
  // GTK initialization
  gtk_init(&argc, &argv);
  
  auto nuscape = Nuscape::getInstance();
 

  return g_application_run(G_APPLICATION(nuscape->app), argc, argv);
}