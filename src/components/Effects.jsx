import { EffectComposer, Bloom, DepthOfField, Vignette, ChromaticAberration } from '@react-three/postprocessing'
import { BlendFunction } from 'postprocessing'

function Effects() {
  return (
    <EffectComposer>
      {/* Bloom for glowing effects */}
      <Bloom
        intensity={0.4}
        luminanceThreshold={0.5}
        luminanceSmoothing={0.9}
        blendFunction={BlendFunction.ADD}
      />

      {/* Vignette for focus */}
      <Vignette
        offset={0.3}
        darkness={0.5}
        blendFunction={BlendFunction.NORMAL}
      />

      {/* Chromatic aberration for realism */}
      <ChromaticAberration
        offset={[0.001, 0.001]}
        blendFunction={BlendFunction.NORMAL}
      />
    </EffectComposer>
  )
}

export default Effects
