const addon = require('../build/Release/tdc-falcon-native');

module.exports = {
    generateKeypair: addon.generateKeypair,
    generateKeypairRandom: addon.generateKeypairRandom,
    sign: addon.sign,
    verify: addon.verify,
    privToPub: addon.privToPub,
};
