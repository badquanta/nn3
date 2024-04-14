SET(ICONS_LIST Back.png Find.png Forward.png Home.png Images.png logo-animated.gif logo-animated2.gif Logo.png Open.png Print.png Reload.png Stop.png)

foreach(ICON_FILE ${ICONS_LIST})
    configure_file(${ICON_FILE} ${ICON_FILE} COPYONLY)    
endforeach(ICON_FILE ${ICONS_LIST})
    
