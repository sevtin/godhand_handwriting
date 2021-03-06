#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DEFALUTFONT "Times New Roman" //default font

#include <QObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QFont>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QTime>
#include <QList>
#include <QStringList>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QUrl>
#include <QLatin1Char>
#include <QPushButton>
#include <QTextStream>
#include <QLabel>

class QMessageBox;
class QPixmap;
class QPainter;
class QPen;
class QFont;
class QImage;
class QPoint;
class QTime;
class QStringList;
class QFileDialog;
class QPushButton;
class QTextStream;
class QLabel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void setRandSource();
    int getRandnum(int);
protected:

private slots:

    void on_PenWidthSbx_editingFinished();

    void on_CharacterSizeTxt_editingFinished();

    void on_CharacterSpaceTxt_editingFinished();

    void on_CharacterExample_editingFinished();

    void on_RandOffsetTxt_editingFinished();

    void on_RandPenWidthSbx_editingFinished();

    void on_RandomScaleTxt_editingFinished();

    void on_randRotateTxt_editingFinished();

    void on_addFontBtn_clicked();

    void on_GenerateExp_triggered();

    void on_Preview_triggered();

    void on_SavePage_triggered();

    void on_Ssingle_triggered();

    void on_removeFontBtn_clicked();

    void on_openPreview_triggered();

    void on_openAction_triggered();

    void on_about_triggered();

    void on_PreferEditFontPixBtn_clicked();

    void on_setFontPixelBtn_clicked();

    void on_SetEditFontBtn_clicked();

    void on_EditFontCbx_currentFontChanged(const QFont &f);

private:
    Ui::MainWindow *ui;
    QList <QImage*> pageList;
    QList <QFont> fontList;
    //set ui string
    void settips();
//    void setArguement();
    //show img in Preview area
    void outPreview(QString *);
    //draw a charactere and raturn it's image
    QImage drawCharacter(QString character, Qt::GlobalColor color = Qt::transparent);
    void generateExp();//generate exmaple character
    void ifErrorDig(bool, QString);
    void generateOutPage();
    bool loadFile();//load file
};

#endif // MAINWINDOW_H
