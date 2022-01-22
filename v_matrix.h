#pragma once

struct cplane_t
{
    Vector normal;
    float dist;
    uint8_t type;   // for fast side tests
    uint8_t signbits;  // signx + (signy<<1) + (signz<<1)
    uint8_t pad[2];

};

class matrix3x4_t
{
public:
    matrix3x4_t() {}
    matrix3x4_t(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23)
    {
        m_flMatVal[0][0] = m00; m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
        m_flMatVal[1][0] = m10; m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
        m_flMatVal[2][0] = m20; m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
    }
    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    void init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
    {
        m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
        m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
        m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
    }
    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
    {
        init(xAxis, yAxis, zAxis, vecOrigin);
    }
    inline void set_origin(Vector const& p)
    {
        m_flMatVal[0][3] = p.x;
        m_flMatVal[1][3] = p.y;
        m_flMatVal[2][3] = p.z;
    }
    inline void invalidate(void)
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                m_flMatVal[i][j] = std::numeric_limits<float>::infinity();
            }
        }
    }
    Vector get_x_axis()  const { return at(0); }
    Vector get_y_axis()  const { return at(1); }
    Vector get_z_axis()  const { return at(2); }
    Vector get_origin() const { return at(3); }
    Vector at(int i) const { return Vector{ m_flMatVal[0][i], m_flMatVal[1][i], m_flMatVal[2][i] }; }
    float* operator[](int i) { return m_flMatVal[i]; }
    const float* operator[](int i) const { return m_flMatVal[i]; }
    float* base() { return &m_flMatVal[0][0]; }
    const float* base() const { return &m_flMatVal[0][0]; }
    float m_flMatVal[3][4];
};
class v_matrix {
public:
    v_matrix();
    v_matrix(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    );

    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    v_matrix(const Vector& forward, const Vector& left, const Vector& up);

    // Construct from a 3x4 matrix
    v_matrix(const matrix3x4_t& matrix3x4);

    // Set the values in the matrix.
    void init(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    );
    // Initialize from a 3x4
    void init(const matrix3x4_t& matrix3x4);

    // array access
    inline float* operator[](int i)
    {
        return m[i];
    }

    inline const float* operator[](int i) const
    {
        return m[i];
    }

    // Get a pointer to m[0][0]
    inline float* base()
    {
        return &m[0][0];
    }

    inline const float* base() const
    {
        return &m[0][0];
    }

    void  set_left(const Vector& vLeft);
    void  set_up(const Vector& vUp);
    void  set_forward(const Vector& vForward);

    void  get_basis_Vectors(Vector& vForward, Vector& vLeft, Vector& vUp) const;
    void  set_basis_Vectors(const Vector& vForward, const Vector& vLeft, const Vector& vUp);

    // Get/Set the translation.
    Vector& get_translation(Vector& vTrans) const;
    void  set_translation(const Vector& vTrans);

    void  pre_translate(const Vector& vTrans);
    void  post_translate(const Vector& vTrans);

    matrix3x4_t& As3x4();
    const matrix3x4_t& As3x4() const;
    void  copy_from3x4(const matrix3x4_t& m3x4);
    void  set3x4(matrix3x4_t& matrix3x4) const;

    bool  operator==(const v_matrix& src) const;
    bool  operator!=(const v_matrix& src) const { return !(*this == src); }

    // Access the basis Vectors.
    Vector  get_left() const;
    Vector  get_up() const;
    Vector  get_forward() const;
    Vector  get_translation() const;


    // Matrix->Vector operations.
public:
    // Multiply by a 3D Vector (same as operator*).
    void  V3Mul(const Vector& vIn, Vector& vOut) const;

    // Multiply by a 4D Vector.
    //void  V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

    // Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
    Vector  apply_rotation(const Vector& vVec) const;

    // Multiply by a Vector (divides by w, assumes input w is 1).
    Vector  operator*(const Vector& vVec) const;

    // Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
    Vector  VMul3x3(const Vector& vVec) const;

    // Apply the inverse (transposed) rotation (only works on pure rotation matrix)
    Vector  VMul3x3Transpose(const Vector& vVec) const;

    // Multiply by the upper 3 rows.
    Vector  VMul4x3(const Vector& vVec) const;

    // Apply the inverse (transposed) transformation (only works on pure rotation/translation)
    Vector  VMul4x3Transpose(const Vector& vVec) const;


    // Matrix->plane operations.
    //public:
    // Transform the plane. The matrix can only contain translation and rotation.
    //void  TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

    // Just calls TransformPlane and returns the result.
    //VPlane  operator*(const VPlane &thePlane) const;

    // Matrix->matrix operations.
public:

    v_matrix& operator=(const v_matrix& mOther);

    // Add two matrices.
    const v_matrix& operator+=(const v_matrix& other);

    // Add/Subtract two matrices.
    v_matrix  operator+(const v_matrix& other) const;
    v_matrix  operator-(const v_matrix& other) const;

    // Negation.
    v_matrix  operator-() const;

    // Return inverse matrix. Be careful because the results are undefined 
    // if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
    v_matrix  operator~() const;

    // Matrix operations.
public:
    // Set to identity.
    void  identity();
    bool  is_identity() const;
public:
    // The matrix.
    float  m[4][4];
};

inline void matrix_get_column(const matrix3x4_t& src, int nCol, Vector& pColumn)
{
    pColumn.x = src[0][nCol];
    pColumn.y = src[1][nCol];
    pColumn.z = src[2][nCol];
}

inline void matrix_position(const matrix3x4_t& matrix, Vector& position)
{
    matrix_get_column(matrix, 3, position);
}