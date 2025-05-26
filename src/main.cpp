#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int WIDTH = 800;
int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* /* window */, double xpos, double ypos) {
  std::cout << "Mouse moved to: " << xpos << ", " << ypos << std::endl;
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
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
  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwSwapInterval(1);

  std::cout << "GPU Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  double lastTime = glfwGetTime();
  int nbFrames = 0;
  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    nbFrames++;

    if (currentTime - lastTime >= 1.0) {  // If last print was more than 1 sec ago
      std::cout << "FPS: " << nbFrames << std::endl;
      nbFrames = 0;
      lastTime += 1.0;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    double center_x = -1.0 + 2 * xpos / WIDTH;
    double center_y = 1.0 - 2 * ypos / HEIGHT;

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(center_x, center_y, 0.0f);

    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(-0.5f, -0.5f, 0.0f);

    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
