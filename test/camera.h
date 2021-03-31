#if !defined(CAMERA_H)
#define CAMERA_H

class Camera {
private:
    double x, y, z;
    double pitch, yaw, roll;
    double speed;

    void MoveDir(double rot);
public:
    Camera();
    Camera(double startX, double startY, double startZ);
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
};


#endif // CAMERA_H
