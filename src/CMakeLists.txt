# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.omenctl\")

add_library(plasma_applet_omenctl MODULE omenctl.cpp)

target_link_libraries(plasma_applet_omenctl
                      Qt5::Gui
                      KF5::Plasma
                      KF5::I18n
                      KF5::Auth
                      cpupower
                      )


install(TARGETS plasma_applet_omenctl DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/applets)

plasma_install_package(package org.kde.plasma.omenctl)

add_executable(omenctl_helper roothelper.h roothelper.cpp)

target_link_libraries(omenctl_helper
                        KF5::Auth
                        Qt5::Core
                     )
install(TARGETS omenctl_helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})

kauth_install_helper_files(omenctl_helper org.kde.omenctl root)
kauth_install_actions(org.kde.omenctl setgovernor.txt)
