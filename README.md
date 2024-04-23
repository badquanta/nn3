# BadQuanta's Nuscape Naviguesser

## DISCLAIMER:

This is a non-commercial project intended only for nostalgia purposes.
I do not make any claim of ownership over the names and logos associated with "Netscape Navigator 3.0."
It appears the Trademark ([Netscape expired in April 2020](https://trademarks.justia.com/746/17/netscape-74617790.html)). 
Logos, icons, and Animations from the original are used only with the purpose of accurately recreating the user interface experience.


## About

As an omage to the web browser I first used to browse the internet and to satiate my own nostalgia
this project aims to recreate the user interface of the "Netscape Navigator 3.0" web browser using GTK3 and WebKit2GTK.

The original Netscape Navigator 3.0 can still be downloaded and installed on modern computers; however it cannot navigate
most modern web pages or deal with current secure browsing standards.  This project aims to allow users a semi-authentic
look and feel of the original browser while supporting today's "World Wide Web."

## Goals

Here is a simple list of what will be implemented eventually:

1. Navigator:  Basic recreation of the main Netscape Navigator browser window.
2. History: An attempt to display the browser history just as Navigator did.
3. Settings: This window will try to be faithful to the original but will be reinterpreted to be more appropriate for WebKit2 settings management.

Here is a list of what __will not__ be implemented (likely ever):

* Email/newsnet mail client.
* WYSIWYG HTML editor.
* Likely any component of the original Navigator 3.0 Suite that is not explicitly listed above.

### Technical Details

The U.I. was recreated using Glade.  The application is implemented mostly in simple C with a few features from C++ sprinkled in for convinece. 
It does not use the gtkmm library only because that proved more difficult to learn and debug than the simpler GTK C library. 

GTK3 is used, instead of GTK4, becuase it more accurately resembles the U.I. of Netscape Navigator 3.

Most of the resources utilized (icons, glade XML, etc) is bundled within the application itself.  It should be sufficent just to copy the executable to where you want it.