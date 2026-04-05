find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2)

qt_add_executable(appTimer main.cpp) # Назва вашого проєкту

target_link_libraries(appTimer
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
)
