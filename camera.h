#if !defined(CAMERA_H)
#define CAMERA_H

class Camera {
private:
    double x, y, z;
    double pitch; // Vertical
    double yaw; // Horizontal
    double roll;
    double speed;

    void DrawText(double x, double y, std::string text);
    void MoveDir(double rot);
public:
    Camera();
    Camera(double startX, double startY, double startZ, double lookAtX, double lookAtY, double lookAtZ);
    ~Camera();

    void Update();

    // Translate camera in direction of rot
    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();

    // Horizontal
    void Yaw(double dRot);
    // Vertical
    void Pitch(double dRot);

    void Jump();

    void ResetView();

    void Display();
    void DisplayInfo(double fps);
};


#endif // CAMERA_H
