# %% Imports and declaration
from ray.rllib.algorithms.ppo import PPOConfig 
from ray.rllib.connectors.env_to_module import FlattenObservations 
import wandb 

wandb.init(project="rllib-demo")

config = (
    PPOConfig()
    .environment('CartPole-v1')
    .env_runners(
        num_env_runners=4, 
        num_envs_per_env_runner=8, 
        env_to_module_connector=lambda env: FlattenObservations(), 
    )
    .evaluation(evaluation_num_env_runners=1)
    .training(
        lr=0.001, clip_param=0.2, 
        gamma=0.995, 
        use_gae=True, lambda_=0.95, 
        use_critic=True, vf_share_layers=True, vf_loss_coeff=1, entropy_coeff=0.001, 
        grad_clip=.5, use_kl_loss=False
    ).evaluation(
        evaluation_interval=1, 
        evaluation_num_env_runners=2, 
        evaluation_duration_unit="episodes", 
        evaluation_duration=10, 
    ).resources(num_gpus=1)
)

algo = config.build_algo()
# %%
from tqdm import trange 

for _ in trange(20):
    wandb.log(algo.train())

wandb.finish()