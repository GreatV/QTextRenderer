#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TextRenderer; }
QT_END_NAMESPACE

class TextRenderer : public QMainWindow
{
    Q_OBJECT

public:
    TextRenderer(QWidget *parent = nullptr);
    ~TextRenderer();

private:
    Ui::TextRenderer *ui;
};
#endif // TEXTRENDERER_H
