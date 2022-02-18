#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
using TexturePtr = shared_ptr<ITexture>;

class Shape : public IShape {
public:
    void SetPosition(Point position) override {
        position_ = position;
    }

    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size size) override {
        size_ = size;
    }

    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) override {
        texture_ = move(texture);
    }

    [[nodiscard]] ITexture* GetTexture() const override {
        return texture_.get();
    }

protected:
    unique_ptr<Shape> CopyFieldsTo(unique_ptr<Shape>&& dest) const {
        (dest)->SetPosition(position_);
        (dest)->SetSize(size_);
        (dest)->SetTexture(texture_);
        return move(dest);
    }
protected:
    TexturePtr texture_;
    Point position_;
    Size size_;

};

bool PointBelongsToImage(Point p, Image& img) {
    return p.x < img.front().size() && p.y < img.size() && p.x >= 0  && p.y >= 0;
}

bool PointBelongsToTexture(Point p, const TexturePtr& texture) {
    if (texture == nullptr)
        return false;
    auto t_wd = texture->GetSize().width;
    auto t_hg = texture->GetSize().height;
    return p.x < t_wd && p.y < t_hg && p.x >= 0 && p.y >= 0;
}

class Rectangle : public Shape {
public:
    [[nodiscard]] unique_ptr<IShape> Clone() const override {
        return CopyFieldsTo(move(make_unique<Rectangle>()));
    }

    // Image - холст на котором рисуем фигуру
    void Draw(Image& image) const override {

        for (int i = 0; i < size_.height; i++) {
            for (int j = 0; j < size_.width; j++) {
                auto img_pixel = Point{j + position_.x, i + position_.y};
                if (PointBelongsToImage(img_pixel, image)) {
                    auto texture_pixel = Point{j, i};
                    if (PointBelongsToTexture(texture_pixel, texture_)) {
                        image[img_pixel.y][img_pixel.x] = texture_->GetImage()[texture_pixel.y][texture_pixel.x];
                    } else {
                        image[img_pixel.y][img_pixel.x] = '.';
                    }
                }
            }
        }

    }
};

class Ellipse : public Shape {
public:
    [[nodiscard]] unique_ptr<IShape> Clone() const override {
        return CopyFieldsTo(move(make_unique<Ellipse>()));
    }


    void Draw(Image& image) const override {

        for (auto i = position_.y; i < image.size(); ++i) {
            if (i - position_.y < size_.height) {
                for (auto j = position_.x; j < image[i].size(); ++j) {
                    if (j - position_.x < size_.width) {
                        if (texture_ == nullptr) {
                            image[i][j] = '.';
                        } else {
                            if (IsPointInEllipse({j - position_.x, i - position_.y}, size_)) {
                                if ((i >= position_.y && i < position_.y + texture_->GetSize().height)
                                    && (j >= position_.x && j < position_.x + texture_->GetSize().width)) {
                                    image[i][j] = texture_->GetImage()[i - position_.y][j - position_.x];
                                } else {
                                    image[i][j] = '.';
                                }
                            }
                        }
                    }
                }
            }
        }
    }

};



// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return make_unique<Rectangle>();
    } else if (shape_type == ShapeType::Ellipse) {
        return make_unique<Ellipse>();
    }
    return nullptr;
}
