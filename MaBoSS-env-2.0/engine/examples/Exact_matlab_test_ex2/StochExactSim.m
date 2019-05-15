function [ProbTick,HTH]=StochExactSim(NbTick,TimeTick,ICond,TransM, Integrator)
PosTransM=TransM;
for k=1:length(TransM(1,:))
    PosTransM(k,k)=0;
end
for i=1:NbTick
    ProbTick(:,i)=repmat(0,[length(ICond) 1]);
    HTH(:,i)=[0 0]';
    for j=1:Integrator
        ProbState=expm(TransM*((i-1)*TimeTick+TimeTick*(j-1)/Integrator))*ICond;
        ProbTick(:,i)=ProbTick(:,i)+ProbState/Integrator;
    end
    HTH(1,i)=entropy(ProbTick(:,i));
    for k=1:length(ProbTick(:,i))
                HTH(2,i)=HTH(2,i)+ProbTick(k,i)*entropy(PosTransM(:,k));
    end
end


        
    