#include "textrenderer.h"
#include "./ui_textrenderer.h"

TextRenderer::TextRenderer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextRenderer)
{
    ui->setupUi(this);
}

TextRenderer::~TextRenderer()
{
    delete ui;
}

