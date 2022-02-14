//This is a general stucture of a Vulkan file
//Taken from the tutorial website (wwww.vulkan-tutorial.com)

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


//Validation Layer
//Creates a const vector with a char pointer, unsure why we need to create a vector
//to store this information
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


class HelloTriangleApplication 
{
public:
	void run()   
	{
		//Vulkan can run without a window. This uses GFLW as the window generator
		initWindow();

		initVulkan();


		//added for my own learning/validating whats in the vector...
		for (int i = 0; i < validationLayers.size(); i++)
		{
			std::cout << validationLayers[i] << "\n";
		} 

		mainloop();
		cleanup();

		
		
	}
	

private:


	//Declare the class window - uses a pointer... 
	//window is the address of type GLFWwindow...why...read GLFW docs
	//This creates a dynamic object and binds its address to a pointer - window;
	GLFWwindow* window;

	//Declare the Vulkan Environement
	//Vulkan must be seen as a subsystem of the application. State must be tracked by the object we create.
	VkInstance instance;

	//Experiment with pointers later...
	//int * testing;
	
	void initWindow()
	{
		std::cout << "Window creation...\n";

		//Intiate the GLFW window library
		glfwInit();

		//GLFW was origonally designed with OpenGL - this call tells it to not create an OpenGL context
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		//Stops the window from being re-sizable
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		//Create the actuall window. GLFW is platform agnostic, so this will work on Windows, Mac or Linux (Linux is where this was written)
		//Parameters: (Width, height, Window Name, Monitor to open window on, OpenGL call (not used in Vulkan))
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);



	}

	void initVulkan()
	{
		std::cout << "Vulkan Starting...\n";

		//This initialises the Vulkan Library
		createInstance();



		std::cout << "Vulkan Stated!\n";

	}

	void mainloop()
	{

		std::cout << "Main Loop begun.\n";

		while(!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		std::cout << "cleanup\n";

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();

	}






	void createInstance()
	{

		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//VkApplicationInfo is a stuct in Vulkan
		//Full details from khronos website
		//Not required to run application. This could be left blank, but probably best to fill it out.

		VkApplicationInfo appInfo{};

		//sType is the type of this structure
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		//pNext is NULL or a pointer to a structure extending the stucture
		//Application is a null/pointer 
		appInfo.pApplicationName = "Hello Triangle";
		//is an unsigned integer variable containing the developer-supplied version number of the application
	    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	    //Can be Null or pointer to a null terminted UFT-8 string containing the name of the engine (if any) used to create the application
	    appInfo.pEngineName = "No Engine";
	    //unsigned int containing the developer-supplied version number of the engine used to create the application
	    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	    //MUST be the highest version of Vulkan that the application is designed to use, uncoded as detailed on khronos website
	    appInfo.apiVersion = VK_API_VERSION_1_0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
	    VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		//These lines return the extensions (to interface with windowing system) from GLFW 
		uint32_t glfwExtensionCount = 0;
	    const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		//Put this GLFW information into the createInfo stuct before passing the the vulkan instance 
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		/////////////TODO/////////////////////////
		if (enableValidationLayers) 			//
		{										//
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    		createInfo.ppEnabledLayerNames = validationLayers.data();
    											//
		} else {								//	
		createInfo.enabledLayerCount = 0;		//
		}										//
		//////////////////////////////////////////	

		//This creates an instance. Remember - you can have more than once instance inside an application
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		std::cout << "Instance Running\n";

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
		{
	    	std::cout << "failed to create instance!";
	    	throw std::runtime_error("failed to create instance!");
		}

		//Debug mode = true && chechValidationLayerSupport 
		if (enableValidationLayers && !checkValidationLayerSupport()) 
		{
        	throw std::runtime_error("validation layers requested, but not available!");
    	} 
    	else if (enableValidationLayers && checkValidationLayerSupport()) 
    	{
    		std::cout << "Debug mode is on and layers found\n";
    	}


	}

	bool checkValidationLayerSupport()
	{
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


        //This is a C++ range based loop
        //The const char pointer named layerName is instanciated here, it will then go through
        //all the elements in validationLayers
        //We set validationLayers at the top of the object, so only 1 loop required
        //Thought: Is un-needed now when only one vaildation layer exists in Vulkan now?
        for (const char* layerName : validationLayers) 
        {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) 
            {
                if (strcmp(layerName, layerProperties.layerName) == 0) 
                {
                    layerFound = true;
                    std::cout << "Layer Found: " << layerProperties.layerName << "\n";
                    break;
                }
            }

            if (!layerFound) 
            {
                return false;
                std::cout << "Layer not Found";
            }
        }

        return true;
    }



};



int main() 
{
	HelloTriangleApplication app;

	try 
	{
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}