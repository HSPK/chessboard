#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    label_w = ui->label->size().width();
    label_h = ui->label->size().height();
    update_data();

    ui->te_debug->append(QString("Width : %1").arg(ui->label->size().width()));
    ui->te_debug->append(QString("Height : %1").arg(ui->label->size().height()));
    ui->te_debug->append(QString("box_w, box_h : %1, %2").arg(box_w).arg(box_h));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_board(int n)
{
    QPainter painter(ui->label);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            painter.setPen(board[i * n + j].l_color);
            painter.setBrush(board[i * n + j].b_color);
            QRect rect(j * box_w, i * box_h, box_w, box_h);
            painter.drawRect(rect);
            if (ui->cb_num->checkState() == Qt::Checked)
                painter.drawText(rect, Qt::AlignCenter, QString("%1").arg(board[i * n + j].num));
        }
    }
}

void MainWindow::clear_board()
{
    struct box box;
    num = 0;
    box.num = 0;
    box.l_color = QColor(0, 0, 0);
    box.b_color = QColor(255, 255, 255, 0);
    board.clear();
    for (int i = 0; i < board_size * board_size; i++)
        board.push_back(box);
    ui->label->update();
}

void MainWindow::draw_l_block(int i, int j, int num, QColor l_color, QColor b_color)
{
    struct box box;
    box.num = num;
    box.l_color = l_color;
    box.b_color = b_color;
    board[i * board_size + j] = box;
    ui->label->update();
}

void MainWindow::update_data()
{
    board_size = 1 << ui->le_board_size->text().toUInt();
    box_w = label_w / board_size;
    box_h = label_h / board_size;
    struct box box;
    num = 0;
    box.num = 0;
    box.l_color = QColor(0, 0, 0);
    box.b_color = QColor(255, 255, 255, 0);
    board.clear();
    for (int i = 0; i < board_size * board_size; i++)
        board.push_back(box);
}

void MainWindow::solve_board(int b_i, int b_j)
{
    _solve_board(0, 0, board_size, b_i, b_j);
}

void MainWindow::_solve_board(int i, int j, int size, int b_i, int b_j)
{
    if (size < 2)
        return;
    int offset = (size - 1) / 2;
    int state = check_b_p(i, j, size, b_i, b_j);
    place_l(i + offset, j + offset, state, QColor(rand() % 256, rand() % 256, rand() % 256));
    size >>= 1;
    if (ui->cb_delay->checkState() == Qt::Checked) {
        ui->label->repaint();
        QThread::msleep(500);
    }
    switch (state) {
    case 0:
        _solve_board(i, j, size, b_i, b_j);
        _solve_board(i, j + size, size, i + offset, j + offset + 1);
        _solve_board(i + size, j, size, i + offset + 1, j + offset);
        _solve_board(i + size, j + size, size, i + offset + 1, j + offset + 1);
        break;
    case 1:
        _solve_board(i, j, size, i + offset, j + offset);
        _solve_board(i, j + size, size, b_i, b_j);
        _solve_board(i + size, j, size, i + offset + 1, j + offset);
        _solve_board(i + size, j + size, size, i + offset + 1, j + offset + 1);
        break;
    case 2:
        _solve_board(i, j, size, i + offset, j + offset);
        _solve_board(i, j + size, size, i + offset, j + offset + 1);
        _solve_board(i + size, j, size, b_i, b_j);
        _solve_board(i + size, j + size, size, i + offset + 1, j + offset + 1);
        break;
    case 3:
        _solve_board(i, j, size, i + offset, j + offset);
        _solve_board(i, j + size, size, i + offset, j + offset + 1);
        _solve_board(i + size, j, size, i + offset + 1, j + offset);
        _solve_board(i + size, j + size, size, b_i, b_j);
        break;
    default:
        break;
    }
}

int MainWindow::check_b_p(int i, int j, int size, int b_i, int b_j)
{
    size >>= 1;
    int ri = b_i - i;
    int rj = b_j - j;
    int res = ((ri / size) << 1) + rj / size;
    ui->te_debug->append(QString("shape : %1").arg(res));
    return res;
}

void MainWindow::place_l(int i, int j, int shape, QColor b_color, QColor l_color)
{
    num++;
    switch (shape) {
    case 0:
        draw_l_block(i, j + 1, num, l_color, b_color);
        draw_l_block(i + 1, j, num, l_color, b_color);
        draw_l_block(i + 1, j + 1, num, l_color, b_color);
        break;
    case 1:
        draw_l_block(i, j, num, l_color, b_color);
        draw_l_block(i + 1, j, num, l_color, b_color);
        draw_l_block(i + 1, j + 1, num, l_color, b_color);
        break;
    case 2:
        draw_l_block(i, j, num, l_color, b_color);
        draw_l_block(i, j + 1, num, l_color, b_color);
        draw_l_block(i + 1, j + 1, num, l_color, b_color);
        break;
    case 3:
        draw_l_block(i, j, num, l_color, b_color);
        draw_l_block(i, j + 1, num, l_color, b_color);
        draw_l_block(i + 1, j, num, l_color, b_color);
        break;
    default:
        break;
    }
//    draw_l_block(i, j, num, l_color, b_color);
//    draw_l_block(i, j + 1, num, l_color, b_color);
//    draw_l_block(i + 1, j, num, l_color, b_color);
//    draw_l_block(i + 1, j + 1, num, l_color, b_color);
//    draw_l_block(i + shape / 2, j + shape % 2, 0, l_color, QColor(0, 0, 0, 0));
//    draw_l_block(b_i, b_j, -1, QColor(0, 0, 0), QColor(160, 50, 80));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label && event->type() == QEvent::Paint) {
        draw_board(board_size);
    }
    if (obj == ui->label && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouse = static_cast<QMouseEvent *>(event);
        if (mouse->buttons() == Qt::LeftButton) {
            // ui->te_debug->append(QString("x : %1, y : %2").arg(mouse->x()).arg(mouse->y()));
            int i = mouse->y() / box_w;
            int j = mouse->x() / box_h;
            ui->te_debug->append(QString("i : %1, j : %2").arg(i).arg(j));
            clear_board();
            b_i = i;
            b_j = j;
            draw_l_block(i, j, -1, QColor(0, 0, 0), QColor(160, 50, 80));
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::on_pushButton_3_clicked()
{
    update_data();
    ui->label->update();
}

void MainWindow::on_cb_num_stateChanged(int arg1)
{
    ui->label->update();
}

void MainWindow::on_pushButton_2_clicked()
{
    clear_board();
}

void MainWindow::on_pushButton_clicked()
{
    solve_board(b_i, b_j);
}
