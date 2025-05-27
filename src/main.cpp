#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <alloca.h>

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

int WIDTH = 800;
int HEIGHT = 600;

static void glClearError() { while (glGetError() != GL_NO_ERROR); }

static void glCheckError() {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error ] (" << error << ")\n";
  }
}

std::string read_shader_file(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "Error: Failed to open shader file: " << filePath << "\n";
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

static unsigned int compile_shader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cerr << "Failed to compile shader of type "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << ": " << message << "\n";
    std::cerr << "Source:\n" << message << "\n";
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int create_shader(const std::string& vertexShader,
                                  const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fixed-function Triangle", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
    return -1;
  }

  glfwSwapInterval(1);

  std::cout << "GPU Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // START HERE

  std::array<float, 8> positions = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
                                    0.5f,  -0.5f, 0.5f

  };

  std::array<unsigned int, 6> indices = {0, 1, 2, 2, 3, 0};

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
               GL_STATIC_DRAW);

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  std::string vertex_shader = read_shader_file("./res/shaders/vertex.glsl");
  std::string fragment_shader = read_shader_file("./res/shaders/fragment.glsl");

  unsigned int shader = create_shader(vertex_shader, fragment_shader);
  glUseProgram(shader);

  int location = glGetUniformLocation(shader, "u_color");
  assert(location != -1);

  float r_chanel = 0.2f;

  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Loop
  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    nbFrames++;

    if (r_chanel >= 1.0f) {
      r_chanel = 0.0f;
    } else {
      r_chanel += 0.001f;
    }

    glUseProgram(shader);
    glUniform4f(location, r_chanel, 0.3f, 0.8f, 1.0f);

    glBindVertexArray(vao);

    if (currentTime - lastTime >= 1.0) {
      std::ostringstream oss;
      oss << "OpenGL Window - FPS: " << nbFrames;
      glfwSetWindowTitle(window, oss.str().c_str());
      nbFrames = 0;
      lastTime += 1.0;
    }

    glClearError();

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glCheckError();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
