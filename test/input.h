#if !defined(INPUT_H)
#define INPUT_H

extern bool keyStates[256];
extern bool specialKeyStates[256];

// Mouse
void mouseClick(int mouse_button, int state, int x, int y);
void mouseMove(int mouseX, int mouseY);
// Keyboard
void keyDown(unsigned char c, int x, int y);
void keyUp(unsigned char c, int x, int y);
void specialKeyDown(int key, int x, int y);
void specialKeyUp(int key, int x, int y);

#endif // INPUT_H
