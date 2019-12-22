//
//  main.cpp
//  OpenGl
//
//  Created by Fan Zhang on 2019/12/16.
//  Copyright © 2019 Fan Zhang. All rights reserved.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderString
{
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderString ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX =0,FRAGMENT =1
    };
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;
    while(getline(stream,line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type]<< line << "\n";
        }
    }
    return {ss[0].str(),ss[1].str()};
}


static unsigned int CompileShader(unsigned int type,const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,NULL);
    glCompileShader(id);
    // to do: Error handling
    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if( result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        std::cout <<"Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
        <<"Shader !"<<std::endl;
        std::cout <<message<<std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader , const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs =CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs =CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}



int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    
    GLFWwindow* window;
     /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    int screenWidth,screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, screenWidth, screenHeight);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK)
    {
        std::cout<<"Error GLEW !"<<std::endl;
    }
    std::cout<< glGetString(GL_VERSION)<<std::endl;
    float position[6]={
        -0.5f,-0.5f,
         0.0f,0.5f,
         0.5f,-0.5f
    };
    unsigned int VAO,VBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),position,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);
    glEnableVertexAttribArray(0);
    
    
    std::string vertexShader =
    "#version 330 core\n"
    "\n"
    "layout( location = 0 ) in vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n";
    
    std::string fragmentShader =
    "#version 330 core\n"
    "\n"
    "layout( location = 0 ) out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1.0f,0.0f,0.0f,1.0f);\n"
    "}\n";
    
    ShaderString myshader;
    
    myshader = ParseShader("/Users/fanzhang/CPPDEV/OpenGL/OpenGl/OpenGl/res/shader/basic.shader");
    
    // print Shader String
    //std::cout<<myshader.FragmentShader<<std::endl;
    //std::cout<<myshader.VertexShader<<std::endl;
    
    unsigned int shader = CreateShader(myshader.VertexShader,myshader.FragmentShader);
    glUseProgram(shader);
    
    //std::cout << fragmentShader<<std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        

        glDrawArrays(GL_TRIANGLES,0,3);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
