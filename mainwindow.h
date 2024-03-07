#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

#define DEFAULT_BN_PRECISION 10

// ****** MAIN WINDOW CLASS ******

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_UnosFirst_textChanged();
    void on_UnosSecond_textChanged();
    void on_I_clicked();
    void on_V_clicked();
    void on_X_clicked();
    void on_L_clicked();
    void on_C_clicked();
    void on_D_clicked();
    void on_M_clicked();
    void on_sabiranje_clicked();
    void on_oduzimanje_clicked();
    void on_mnozenje_clicked();
    void on_deljenje_clicked();

    void on_UnosFirst_vb_textChanged();
    void on_UnosSecond_vb_textChanged();
    void on_nula_clicked();
    void on_jedan_clicked();
    void on_dva_clicked();
    void on_tri_clicked();
    void on_cetiri_clicked();
    void on_pet_clicked();
    void on_sest_clicked();
    void on_sedam_clicked();
    void on_osam_clicked();
    void on_devet_clicked();
    void on_coma_clicked();
    void on_sabiranje_vb_clicked();
    void on_oduzimanje_vb_clicked();
    void on_mnozenje_vb_clicked();
    void on_deljenje_vb_clicked();

    void on_Rimski_clicked();
    void on_BigNumber_clicked();

private:
    Ui::MainWindow *ui;
    bool get_roman_op_input(int &a, int &b);
    unsigned int get_precision();
};

// ****** CUSTOM CLASSES AND FUNCTIONS ******

int toArabic(const QString &RomanNumber);
QString toRoman(int ArabicNumber);
QString saberi_vb(const QString &prvi, const QString &drugi, unsigned int precision);
QString oduzmi_vb(const QString &prvi, const QString &drugi, unsigned int precision);
QString podeli_vb(const QString &prvi, const QString &drugi, unsigned int precision);
QString pomnozi_vb(const QString &prvi, const QString &drugi, unsigned int precision);

// ****** CUTSOM CLASSES ******

class QMyButton : public QPushButton
{
public:
    using QPushButton::QPushButton;
protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
};

class QUnosBroja : public QTextEdit
{
    Q_OBJECT
public:
    using QTextEdit::QTextEdit;
    bool UnosOvde = false;
    QString pozicija;
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
signals:
    void focusIn();
public slots:
    void respondToOtherInFocus();
};

#endif // MAINWINDOW_H
