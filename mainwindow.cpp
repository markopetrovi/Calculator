#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFocusEvent>
#include <QSysInfo>
#include <QWidget>
#include <QIntValidator>

void hide_layout(QLayout *layout)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        if (item) {
            QWidget *widget = item->widget();
            if (widget)
                widget->hide();
        }
    }
}

void show_layout(QLayout *layout)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        if (item) {
            QWidget *widget = item->widget();
            if (widget)
                widget->show();
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIntValidator *validator = new QIntValidator(this);
    ui->precision->setValidator(validator);
    ui->precision->setPlaceholderText(QString::number(DEFAULT_BN_PRECISION));
    auto configureInput = [](QUnosBroja *prvi, QUnosBroja *drugi) {
        prvi->pozicija = "prvi";
        drugi->pozicija = "drugi";
        prvi->setPlaceholderText(QStringLiteral("Unesite prvi broj..."));
        drugi->setPlaceholderText(QStringLiteral("Unesite drugi broj..."));
        connect(prvi, &QUnosBroja::focusIn, drugi, &QUnosBroja::respondToOtherInFocus);
        connect(drugi, &QUnosBroja::focusIn, prvi, &QUnosBroja::respondToOtherInFocus);
        if (QSysInfo::productType() == "android") {
            prvi->setReadOnly(true);
            drugi->setReadOnly(true);
        }
    };
    configureInput(ui->UnosFirst, ui->UnosSecond);
    configureInput(ui->UnosFirst_vb, ui->UnosSecond_vb);

    /* Setup for Roman Numbers */
    QWidget::setTabOrder(ui->I, ui->V);
    QWidget::setTabOrder(ui->V, ui->X);
    QWidget::setTabOrder(ui->X, ui->L);
    QWidget::setTabOrder(ui->L, ui->C);
    QWidget::setTabOrder(ui->C, ui->D);
    QWidget::setTabOrder(ui->D, ui->M);
    QWidget::setTabOrder(ui->M, ui->sabiranje);
    QWidget::setTabOrder(ui->sabiranje, ui->oduzimanje);
    QWidget::setTabOrder(ui->oduzimanje, ui->mnozenje);
    QWidget::setTabOrder(ui->mnozenje, ui->deljenje);

    /* Setup for Big Numbers */
    QWidget::setTabOrder(ui->jedan, ui->dva);
    QWidget::setTabOrder(ui->dva, ui->tri);
    QWidget::setTabOrder(ui->tri, ui->cetiri);
    QWidget::setTabOrder(ui->cetiri, ui->pet);
    QWidget::setTabOrder(ui->pet, ui->sest);
    QWidget::setTabOrder(ui->sest, ui->sedam);
    QWidget::setTabOrder(ui->sedam, ui->osam);
    QWidget::setTabOrder(ui->osam, ui->devet);
    QWidget::setTabOrder(ui->devet, ui->coma);
    QWidget::setTabOrder(ui->coma, ui->sabiranje_vb);
    QWidget::setTabOrder(ui->sabiranje_vb, ui->oduzimanje_vb);
    QWidget::setTabOrder(ui->oduzimanje_vb, ui->mnozenje_vb);
    QWidget::setTabOrder(ui->mnozenje_vb, ui->deljenje_vb);

    on_Rimski_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Add enter as a valid click button */
void QMyButton::keyPressEvent(QKeyEvent *e)
{
    QPushButton::keyPressEvent(e);
    if (e->key() == Qt::Key_Enter)
        click();
}

/* Set valid placeholders based on focus */

void QUnosBroja::focusInEvent(QFocusEvent *e)
{
    if (e->gotFocus())
    {
        this->setPlaceholderText("Unosi se " + pozicija + " broj...");
        if (QSysInfo::productType() == "android")
            this->clearFocus();
        else
            QTextEdit::focusInEvent(e);
        UnosOvde = true;
        emit focusIn();
    }
}

void QUnosBroja::respondToOtherInFocus()
{
    this->setPlaceholderText("Unesite " + pozicija + " broj...");
    UnosOvde = false;
}

/* UI slots */
// ****** INPUT ******
void buttonLogic(QUnosBroja *editbox, const QString &character)
{
    QTextCursor cursor = editbox->textCursor();
    QString text = editbox->toPlainText();
    int pos = cursor.anchor();
    int pos1 = cursor.position();
    if (pos > pos1) {
        int p = pos1;
        pos1 = pos;
        pos = p;
    }
    if (pos != pos1)
        text.remove(pos, pos1 - pos);
    text.insert(pos, character);
    editbox->setPlainText(text);
    cursor.setPosition(pos+1);
    editbox->setTextCursor(cursor);
}

#define createSlot(button, sufix, buttonChar)               \
    void MainWindow::on_##button##_clicked()                \
    {                                                       \
        if (ui->UnosFirst##sufix->UnosOvde)                 \
            buttonLogic(ui->UnosFirst##sufix, #buttonChar); \
        if (ui->UnosSecond##sufix->UnosOvde)                \
            buttonLogic(ui->UnosSecond##sufix, #buttonChar);\
    }

createSlot(I,,I)
createSlot(V,,V)
createSlot(X,,X)
createSlot(L,,L)
createSlot(C,,C)
createSlot(D,,D)
createSlot(M,,M)
createSlot(nula, _vb, 0)
createSlot(jedan, _vb, 1)
createSlot(dva, _vb, 2)
createSlot(tri, _vb, 3)
createSlot(cetiri, _vb, 4)
createSlot(pet, _vb, 5)
createSlot(sest, _vb, 6)
createSlot(sedam, _vb, 7)
createSlot(osam, _vb, 8)
createSlot(devet, _vb, 9)
createSlot(coma, _vb, .)

inline bool is_valid_RomanDigit(QChar c)
{
    return (c == 'I' || c == 'V' || c == 'X' || c == 'L' || c == 'C' || c == 'D' || c == 'M');
}
inline bool is_valid_Digit(QChar c)
{
    return (c >= '0' && c <= '9') || c == ',' || c == '.';
}

#define createChangedSlot(editbox, type)                                    \
void MainWindow::on_##editbox##_textChanged()                               \
{                                                                           \
    static bool triggered_by_setPlainText = false;                          \
    if (triggered_by_setPlainText) {                                        \
        triggered_by_setPlainText = false;                                  \
        return;                                                             \
    }                                                                       \
                                                                            \
    const QString text = ui->editbox->toPlainText().toUpper();              \
    QTextCursor cursor = ui->editbox->textCursor();                         \
    QString newText;                                                        \
    for (int i = 0; i < text.size(); i++) {                                 \
        if (is_valid_##type##Digit(text[i]))                                \
            newText += text[i];                                             \
    }                                                                       \
    triggered_by_setPlainText = true;                                       \
    int newPos = cursor.position() - (text.size() - newText.size());        \
    if (newPos < 0)                                                         \
        newPos = 0;                                                         \
    ui->editbox->setPlainText(newText);                                     \
    cursor.setPosition(newPos);                                             \
    ui->editbox->setTextCursor(cursor);                                     \
}
createChangedSlot(UnosFirst, Roman)
createChangedSlot(UnosSecond, Roman)
createChangedSlot(UnosFirst_vb, )
createChangedSlot(UnosSecond_vb, )

// ****** OUTPUT ******
bool MainWindow::get_roman_op_input(int &a, int &b)
{
    a = toArabic(ui->UnosFirst->toPlainText());
    b = toArabic(ui->UnosSecond->toPlainText());
    if (a < 0) {
        ui->Rezultat->setHtml(QStringLiteral("<b>Prvi</b> broj nije validan rimski broj"));
        return false;
    }
    if (b < 0) {
        ui->Rezultat->setHtml(QStringLiteral("<b>Drugi</b> broj nije validan rimski broj"));
        return false;
    }
    return true;
}

void MainWindow::on_sabiranje_clicked()
{
    int a, b;
    if (!get_roman_op_input(a, b))
        return;
    a += b;
    ui->Rezultat->setHtml(toRoman(a));
}
void MainWindow::on_oduzimanje_clicked()
{
    int a, b;
    if (!get_roman_op_input(a, b))
        return;
    a -= b;
    ui->Rezultat->setHtml(toRoman(a));
}
void MainWindow::on_mnozenje_clicked()
{
    int a, b;
    if (!get_roman_op_input(a, b))
        return;
    a *= b;
    ui->Rezultat->setHtml(toRoman(a));
}
void MainWindow::on_deljenje_clicked()
{
    int a, b;
    if (!get_roman_op_input(a, b))
        return;
    int rez = a / b;
    int ost = a % b;
    ui->Rezultat->setHtml(QStringLiteral("Rezultat: ") + toRoman(rez) + "<br>" + QStringLiteral("Ostatak: ") + toRoman(ost));
}

void MainWindow::on_Rimski_clicked()
{
    hide_layout(ui->gridLayout_vb);
    ui->UnosFirst_vb->hide();
    ui->UnosSecond_vb->hide();
    show_layout(ui->gridLayout);
    ui->UnosFirst->show();
    ui->UnosSecond->show();
    ui->Rimski->setStyleSheet(QStringLiteral("background-color: rgb(143, 240, 164);"));
    ui->BigNumber->setStyleSheet("");
    ui->Rezultat->setPlainText("");
}

void MainWindow::on_BigNumber_clicked()
{
    hide_layout(ui->gridLayout);
    ui->UnosFirst->hide();
    ui->UnosSecond->hide();
    show_layout(ui->gridLayout_vb);
    ui->UnosFirst_vb->show();
    ui->UnosSecond_vb->show();
    ui->BigNumber->setStyleSheet(QStringLiteral("background-color: rgb(143, 240, 164);"));
    ui->Rimski->setStyleSheet("");
    ui->Rezultat->setPlainText("");
}

unsigned int MainWindow::get_precision()
{
    QString p = ui->precision->text();
    if (p.isEmpty())
        return DEFAULT_BN_PRECISION;
    return p.toInt();
}

void MainWindow::on_sabiranje_vb_clicked()
{
    ui->Rezultat->setHtml( saberi_vb(ui->UnosFirst_vb->toPlainText(), ui->UnosSecond_vb->toPlainText(), get_precision()) );
}

void MainWindow::on_oduzimanje_vb_clicked()
{
    ui->Rezultat->setHtml( oduzmi_vb(ui->UnosFirst_vb->toPlainText(), ui->UnosSecond_vb->toPlainText(), get_precision()) );
}

void MainWindow::on_mnozenje_vb_clicked()
{
    ui->Rezultat->setHtml( pomnozi_vb(ui->UnosFirst_vb->toPlainText(), ui->UnosSecond_vb->toPlainText(), get_precision()) );
}

void MainWindow::on_deljenje_vb_clicked()
{
    ui->Rezultat->setHtml( podeli_vb(ui->UnosFirst_vb->toPlainText(), ui->UnosSecond_vb->toPlainText(), get_precision()) );
}
