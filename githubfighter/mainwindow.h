#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>

#define Interval    20
#define Speed       5
#define MaxGithub   20

class Github : public QGraphicsPixmapItem
{
public:
    Github(qreal width) {
        setPos(QRandomGenerator::global()->bounded(width), 0);
        setPixmap(QPixmap(":/GitHub-Mark-32px.png"));
    }
    virtual ~Github() {}
};

class Bullet : public QGraphicsEllipseItem
{
public:
    Bullet(const QPointF& startAt) {
        setRect(0, 0, 5, 8);
        setPos(startAt);
        setBrush(QBrush(Qt::black));
    }
    virtual ~Bullet() {}
};

class Fighter : public QGraphicsPixmapItem
{
public:
    Fighter() {
        setActive(false);
        setPixmap(QPixmap(":/3Mzz9.jpg"));
    }
    virtual ~Fighter() {}

    Bullet* attack() {
        const QSizeF& s = boundingRect().size()/2;
        return new Bullet(pos() + QPointF(s.width(), s.height()));
    }

    void moveBy(const QRectF& sceneRect, const QPointF& offset) {
        QPointF newPos = pos() + QPointF(offset.x(), offset.y());
        if(sceneRect.contains(QRectF(newPos, boundingRect().size())))
            setPos(newPos);
    }
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene() {
        connect(&mClock, &QTimer::timeout, this, &Scene::handler);
    }
    virtual ~Scene() {}

    void start() {
        mPressedKeys.clear();
        clear();
        addFighter();
        mClock.start(Interval);
    }

    void stop() {
        mClock.stop();
        QMessageBox msgBox;
        msgBox.setText("Scores: " + QString::number(mScore));
        msgBox.setInformativeText("Do you want to restart the game?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
            start();
        else
            qApp->quit();
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        if(event->key() == Qt::Key_Up ||
           event->key() == Qt::Key_Down ||
           event->key() == Qt::Key_Left ||
           event->key() == Qt::Key_Right ||
           event->key() == Qt::Key_X)
            mPressedKeys += event->key();
    }
    void keyReleaseEvent(QKeyEvent *event) override {
        if( event->key() == Qt::Key_Up ||
            event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_Left ||
            event->key() == Qt::Key_Right ||
            event->key() == Qt::Key_X)
            mPressedKeys -= event->key();
    }
private slots:
    void handler() {
        if(mFighter != Q_NULLPTR) {
            controlFighter();
            collision();
            addGithub();
        }
    }

private:
    QTimer              mClock;
    Fighter*            mFighter;
    int                 mG;
    int                 mScore;

    QSet<int>           mPressedKeys;

    void addFighter() {
        mFighter = new Fighter();
        addItem(mFighter);
        mFighter->setPos(sceneRect().center());
        mScore = 0;
    }

    void addBullet() {
        addItem(mFighter->attack());
    }

    void addGithub() {
        if(mG > MaxGithub){
            addItem(new Github(width()));
            mG = 0;
        } else {
            mG++;
        }
    }

    void collision() {
        QVector<Bullet*> bullets;
        QVector<Github*> githubs;
        foreach (QGraphicsItem* item, items()) {
            Bullet* b = qgraphicsitem_cast<Bullet*>(item);
            Github* g = dynamic_cast<Github*>(item);
            if(b != Q_NULLPTR) {
                if(b->y() > 0) {
                    b->moveBy(0, -3 * Speed);
                    bullets.push_back(b);
                } else {
                    removeItem(b);
                    delete b;
                }
            } else if (g != Q_NULLPTR) {
                if(g->y() < height()) {
                    if(g->collidesWithItem(mFighter)) {
                        stop();
                        return;
                    } else {
                        g->moveBy(0, Speed);
                        githubs.push_back(g);
                    }
                } else {
                    removeItem(g);
                    delete g;
                }
            }
        }

        QSet<QGraphicsItem*> dead;
        foreach (Bullet* b, bullets) {
            foreach (Github* g, githubs) {
                if(g != Q_NULLPTR && b != Q_NULLPTR)
                    if(g->collidesWithItem(b)) {
                        bullets.removeOne(b);
                        githubs.removeOne(g);
                        dead.insert(b);
                        dead.insert(g);
                        mScore++;
                    }
            }
        }

        foreach (QGraphicsItem* item, dead) {
            removeItem(item);
            delete item;
        }
    }

    void controlFighter() {
        QPointF offset(0, 0);
        foreach (int key, mPressedKeys) {
            switch (key) {
            case Qt::Key_Up:
                offset += QPointF(0, -Speed);
                break;
            case Qt::Key_Down:
                offset += QPointF(0, Speed);
                break;
            case Qt::Key_Left:
                offset += QPointF(-Speed, 0);
                break;
            case Qt::Key_Right:
                offset += QPointF(Speed, 0);
                break;
            case Qt::Key_X:
                addBullet();
                break;
            default:
                break;
            }
        }

        mFighter->moveBy(sceneRect(), offset);
    }

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGraphicsView   mView;
    Scene           mScene;
protected:
    void resizeEvent(QResizeEvent* e);
    void showEvent(QShowEvent* e);
};

#endif // MAINWINDOW_H
