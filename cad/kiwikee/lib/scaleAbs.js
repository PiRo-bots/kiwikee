const {translate, scale} = require('@jscad/csg/api').transformations

const scaleAbs = (target, shapes) => {

}

// modified versions of johnwebbcole/jscad-utils

const scaleBy = function scale (size, value) {
  if (value === 0) return 1

  return 1 + 100 / (size / value) / 100
}

/**
 * Returns a `Vector3D` with the size of the object.
 * @param  {CSG} o A `CSG` like object or an array of `CSG.Vector3D` objects (the result of getBounds()).
 * @return {CSG.Vector3D}   Vector3d with the size of the object
 */
const measureSize = shape => {
  const bbox = require('../utils/bounds')(shape)
  return bbox[1].minus(bbox[0])
}

const getCentroid = function (shape, size) {
  const bbox = require('../utils/bounds')
  size = size || measureSize(shape)

  return bbox[0].plus(size.dividedBy(2))
}

const map = function (o, f) {
  return Object.keys(o).map(function (key) {
    return f(o[key], key, o)
  })
}

/**
 * Enlarge an object by scale units, while keeping the same
 * centroid.  For example util.enlarge(o, 1, 1, 1) enlarges
 * object o by 1mm in each access, while the centroid stays the same.
 * @param  {CSG} object [description]
 * @param  {number} x      [description]
 * @param  {number} y      [description]
 * @param  {number} z      [description]
 * @return {CSG}        [description]
 */
const enlarge = function enlarge (scaleAmount, shape) {
  const size = measureSize(shape)
  const centroid = getCentroid(shape, size)

  let idx = 0

  const t = map(size, function (i) {
    return scaleBy(i, scaleAmount[idx++])
  })

  const scaledShape = scale(t, shape)
  const scaledCentroid = getCentroid(scaledShape)

  /// Calculate the difference between the original centroid and the new
  const delta = scaledCentroid.minus(centroid).times(-1)

  return translate(delta, scaledShape)
}

module.exports = {scaleBy, enlarge}
