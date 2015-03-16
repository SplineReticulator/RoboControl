#include "ui_visual3d.h"

Object3D::Object3D() {

    numVertices = 0;
    positions = NULL;
    colors = NULL;
    barys = NULL;
}
Object3D::~Object3D() {}

void Object3D::load(QString obj) {

    QFile file(obj);
    if (file.open(QIODevice::ReadOnly)) {

        QTextStream ts(&file);

        while (!ts.atEnd()) {
            QString line = ts.readLine();
            QString type = line.section(' ', 0, 0);
            if (type == "c") {
                c.setX(line.section(' ', 1, 1).toFloat());
                c.setY(line.section(' ', 2, 2).toFloat());
                c.setZ(line.section(' ', 3, 3).toFloat());
            }
            else if (type == "v") {
                v << line.section(' ', 1, 1).toFloat();
                v << line.section(' ', 2, 2).toFloat();
                v << line.section(' ', 3, 3).toFloat();
            }
            else if (type == "f") {
                f << line.section(' ', 1, 1).toUInt();
                f << line.section(' ', 2, 2).toUInt();
                f << line.section(' ', 3, 3).toUInt();
            }
            else if (type == "b") {
                b << line.section(' ', 1, 1).toUInt();
                b << line.section(' ', 2, 2).toUInt();
                b << line.section(' ', 3, 3).toUInt();
            }
            else {
                // unrecognized type
            }
        }

        file.close();
    }
    else {
        // obj file error
    }

    if ((numVertices = f.length()) > 0) {

        positions = new GLfloat[numVertices*3];
        colors = new GLfloat[numVertices*3];
        barys = new GLfloat[numVertices*3];

        for (int i = 0; i < numVertices*3; i++) {
            positions[i] = v[(f[(i/3)]-1)*3+(i%3)];
            colors[i] = 1.0f;
            barys[i] =  b[(i/3)] && (i%9 == 0 || i%9 == 4 || i%9 == 8) ? 1 : 0;
        }
    }
}

void Object3D::setParent(Object3D* parent) {

    this->parent = parent;
}

void Object3D::setBase(QVector3D p, QQuaternion q) {

    base_p = p;
    base_q = q;

    transform(QVector3D(), QQuaternion());
}

void Object3D::transform(QVector3D p, QQuaternion q) {

    model_m.setToIdentity();

    if (parent != NULL) {
        model_m.translate(parent->c);
        model_m = parent->model_m * model_m;
    }

    model_m.translate(p);
    model_m.translate(base_p);
    model_m.rotate(q);
    model_m.rotate(base_q);
    model_m.translate(-c);
}

Visual3D::Visual3D(QString title) {

    // ===== GUI stuff ===== //
    group = new QGroupBox(title);
    layout = new QGridLayout();

    setMaximumSize(440, 440);
    setMinimumSize(220, 220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont pt7;
    pt7.setPointSize(7);

    stateLabel = new QLabel("[Status message]");
    stateLabel->setFont(pt7);

    layout->addWidget(this, 0, 0);
    layout->addWidget(stateLabel, 1, 0, Qt::AlignRight|Qt::AlignTop);

    group->setLayout(layout);

    installEventFilter(this);

    // ===== OpenGL stuff ===== //
    defineModel();

    defaultViewNormal = QVector3D(0, 0, -1);

    x0 = 0;
    y0 = 0;
    dir = 0;
    delay = 0;
}
Visual3D::~Visual3D() {}

bool Visual3D::eventFilter(QObject* watched, QEvent* event) {

    Q_UNUSED(watched);
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    int dx, dy;

    switch (event->type()) {

    case QEvent::MouseButtonPress:

        if (mouseEvent->button() == Qt::LeftButton) {
            x0 = mouseEvent->x();
            y0 = mouseEvent->y();
            dir = 0;
            delay = 0;
        }
        else if (mouseEvent->button() == Qt::RightButton) {
            resizeGL(width(), height());
        }
        return true;

    case QEvent::MouseMove:

        dx = mouseEvent->x()-x0;
        dy = mouseEvent->y()-y0;

        if (delay >= 5) {
            dir = (dir == 0) ? (dir = dx*dx > dy*dy ? 1 : 2) : dir;
        }
        else {
            delay++;
            return true;
        }

        (viewNormal_v = defaultViewNormal*view_m).normalize();

        switch (dir) {
        case 1:
            perspective_m.rotate(dx/2.f, QVector3D(0, 1, 0));
            view_m.rotate(dx/2.f, QVector3D(0, 1, 0));
            break;
        case 2:
            perspective_m.rotate(dy/2.f, QVector3D::normal(viewNormal_v, QVector3D(0, 1, 0)));
            break;
        default:
            break;
        }

        updateGL();

        x0 = mouseEvent->x();
        y0 = mouseEvent->y();

        return true;

    default:
        return false;
    }
    return false;
}

void Visual3D::update(Indicator* ind) {

    // Make sure transforms are called in hierarchical order (i.e., root node first, then second level, third level, etc.)

    objects[Object3D::BODY].transform(QVector3D(), eulerToQuaternion(ind[Indicator::EULER_ANGLE_1].f, ind[Indicator::EULER_ANGLE_2].f, ind[Indicator::EULER_ANGLE_3].f));

    objects[Object3D::FLIPPER_FRONT].transform(QVector3D(), eulerToQuaternion(0, ind[Indicator::FLIPPER_FRONT_POSITION].f, 0));
    objects[Object3D::FLIPPER_BACK].transform(QVector3D(), eulerToQuaternion(0, ind[Indicator::FLIPPER_BACK_POSITION].f, 0));

    updateGL();
}

void Visual3D::initializeGL() {

    gl_program = new QOpenGLShaderProgram();
    gl_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/visualxv.gls");
    gl_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/visualxf.gls");
    gl_program->link();

    position_loc = gl_program->attributeLocation("position");
    color_loc = gl_program->attributeLocation("color");
    barycentric_loc = gl_program->attributeLocation("barycentric");

    perspective_m_loc = gl_program->uniformLocation("perspective_m");
    view_m_loc = gl_program->uniformLocation("view_m");
    model_m_loc = gl_program->uniformLocation("model_m");

    glClearColor(0.85f, 0.85f, 0.9f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Visual3D::resizeGL(int w, int h) {

    glViewport(0, 0, (GLint)w, (GLint)h);

    perspective_m.setToIdentity();
    perspective_m.perspective(60.0f, (float)w/h, 10.0f, 10000.0f);
    perspective_m.translate(0, 0, -1200.0);

    view_m.setToIdentity();

    updateGL();
}

void Visual3D::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl_program->bind();

    glUniformMatrix4fv(perspective_m_loc, 1, GL_FALSE, perspective_m.data());
    glUniformMatrix4fv(view_m_loc, 1, GL_FALSE, view_m.data());

    for (int ref = 0; ref < Object3D::COUNT; ref++) {
        if (objects[ref].numVertices > 0) {

            glUniformMatrix4fv(model_m_loc, 1, GL_FALSE, objects[ref].model_m.data());

            glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, objects[ref].positions);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, objects[ref].colors);
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(barycentric_loc, 3, GL_FLOAT, GL_FALSE, 0, objects[ref].barys);
            glEnableVertexAttribArray(2);

            glDrawArrays(GL_TRIANGLES, 0, objects[ref].numVertices);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
    }

    gl_program->release();
}

void Visual3D::defineModel() {

    objects[Object3D::BODY].load(":/models/body.obj");
    objects[Object3D::BODY].setParent(NULL);
    objects[Object3D::BODY].setBase(QVector3D(), QQuaternion());

    objects[Object3D::FLIPPER_FRONT].load(":/models/flippers.obj");
    objects[Object3D::FLIPPER_FRONT].setParent(&objects[Object3D::BODY]);
    objects[Object3D::FLIPPER_FRONT].setBase(QVector3D(-300, 0, 0), eulerToQuaternion(0, 180, 0));

    objects[Object3D::FLIPPER_BACK].load(":/models/flippers.obj");
    objects[Object3D::FLIPPER_BACK].setParent(&objects[Object3D::BODY]);
    objects[Object3D::FLIPPER_BACK].setBase(QVector3D(300, 0, 0), eulerToQuaternion(0, 0, 0));
}

QQuaternion Visual3D::eulerToQuaternion(qreal phi, qreal theta, qreal psi) {

    qreal c1 = qCos((phi/2*(M_PI/180)));
    qreal c2 = qCos((theta/2)*(M_PI/180));
    qreal c3 = qCos((psi/2)*(M_PI/180));
    qreal s1 = qSin((phi/2)*(M_PI/180));
    qreal s2 = qSin((theta/2)*(M_PI/180));
    qreal s3 = qSin((psi/2)*(M_PI/180));

    return QQuaternion(c1*c2*c3-s1*s2*s3, s1*s2*c3+c1*c2*s3, s1*c2*c3+c1*s2*s3, c1*s2*c3-s1*c2*s3);
}
