#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    struct box{
        int num;
        QColor b_color;
        QColor l_color;
    }box;
    std::vector<struct box> board;
    int board_size;
    int label_w;
    int label_h;
    int box_w;
    int box_h;
    int b_i = 0;
    int b_j = 0;
    int num = 0;
    void draw_board(int n);
    void clear_board();
    void draw_l_block(int i, int j, int num = 0, QColor l_color = QColor(0, 0, 0), QColor b_color = QColor(0, 0, 0));
    void update_data();
    void solve_board(int b_i, int b_j);
    void _solve_board(int i, int j, int size, int b_i, int b_j);
    int check_b_p(int i, int j, int size, int b_i, int b_j);
    void place_l(int i, int j, int shape, QColor b_color, QColor l_color = QColor(0, 0, 0));

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void draw();
private slots:
    void on_pushButton_3_clicked();
    void on_cb_num_stateChanged(int arg1);
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
